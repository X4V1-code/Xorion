#include "DiscordPresence.h"

#include "../../Utils/Logger.h"
#include "../../../Memory/GameData.h"
#include "../../../SDK/LocalPlayer.h"
#include "../../../SDK/RakNetConnector.h"

#include <cstdlib>
#include <ctime>
#include <memory>

static constexpr const char* LARGE_IMAGE_KEY = "xorion";
static constexpr const char* LARGE_IMAGE_TEXT = "Xorion Client";

DiscordPresence::DiscordPresence()
    : IModule(0x0, Category::CLIENT, "Share your Xorion status on Discord") {}

DiscordPresence::~DiscordPresence() {
    shutdownPresence();
}

const char* DiscordPresence::getModuleName() {
    return "DiscordPresence";
}

std::string DiscordPresence::resolveAppId() const {
#ifdef _MSC_VER
    char* rawValue = nullptr;
    size_t len = 0;
    if (_dupenv_s(&rawValue, &len, "XORION_DISCORD_APP_ID") == 0 && rawValue && len > 0) {
        std::unique_ptr<char, decltype(&free)> envValue(rawValue, &free);
        std::string value(envValue.get());
        while (!value.empty() && (value.back() == '\0' || value.back() == '\n' || value.back() == '\r'))
            value.pop_back();
        if (!value.empty())
            return value;
    }
#else
    if (const char* envValue = std::getenv("XORION_DISCORD_APP_ID")) {
        if (*envValue != '\0')
            return envValue;
    }
#endif
    return {};
}

void DiscordPresence::loadLibrary() {
    if (discordLib != nullptr)
        return;

    discordLib = LoadLibraryA("discord-rpc.dll");
    if (discordLib == nullptr) {
        logF("[DiscordPresence] discord-rpc.dll not found, Rich Presence disabled.");
        return;
    }

    initializeFn = reinterpret_cast<DiscordInitializeFn>(GetProcAddress(discordLib, "Discord_Initialize"));
    updateFn = reinterpret_cast<DiscordUpdatePresenceFn>(GetProcAddress(discordLib, "Discord_UpdatePresence"));
    clearFn = reinterpret_cast<DiscordClearPresenceFn>(GetProcAddress(discordLib, "Discord_ClearPresence"));
    shutdownFn = reinterpret_cast<DiscordShutdownFn>(GetProcAddress(discordLib, "Discord_Shutdown"));
    callbacksFn = reinterpret_cast<DiscordRunCallbacksFn>(GetProcAddress(discordLib, "Discord_RunCallbacks"));

    if (!initializeFn || !updateFn || !clearFn || !shutdownFn || !callbacksFn) {
        logF("[DiscordPresence] Missing Discord RPC exports, Rich Presence disabled.");
        shutdownPresence();
    }
}

void DiscordPresence::ensureInitialized() {
    if (ready)
        return;

    loadLibrary();
    if (!initializeFn)
        return;

    DiscordEventHandlers handlers{};
    const auto appId = resolveAppId();
    if (appId.empty()) {
        if (!warnedFallbackId) {
            logF("[DiscordPresence] No Discord App ID configured. Set XORION_DISCORD_APP_ID to your Discord application id that includes a Rich Presence asset named 'xorion'.");
            warnedFallbackId = true;
        }
        setEnabled(false);
        return;
    }
    initializeFn(appId.c_str(), &handlers, 1, nullptr);

    ready = true;
    startTimestamp = std::time(nullptr);
    updatePresence();
}

void DiscordPresence::updatePresence() {
    if (!ready || !updateFn)
        return;

    lastDetails = "In menus";
    lastState = "Xorion Client";

    if (g_Data.isInGame()) {
        if (auto connector = g_Data.getRakNetConnector(); connector && connector->isOnAServer()) {
            lastDetails = "On a server";
        } else {
            lastDetails = "Singleplayer world";
        }

        if (auto lp = g_Data.getLocalPlayer()) {
            if (auto name = lp->getNameTag()) {
                if (auto text = name->getText(); text && *text != '\0')
                    lastState = "Playing as " + std::string(text);
                else
                    lastState = "Playing in-world";
            }
        } else {
            lastState = "In world";
        }
    }

    DiscordRichPresence presence{};
    presence.details = lastDetails.c_str();
    presence.state = lastState.c_str();
    presence.startTimestamp = startTimestamp;
    presence.largeImageKey = LARGE_IMAGE_KEY;
    presence.largeImageText = LARGE_IMAGE_TEXT;

    updateFn(&presence);
}

void DiscordPresence::clearPresence() {
    if (ready && clearFn)
        clearFn();
}

void DiscordPresence::shutdownPresence() {
    const bool hadPresence = ready;
    ready = false;

    if (hadPresence && clearFn)
        clearFn();

    if (hadPresence && shutdownFn)
        shutdownFn();

    if (discordLib != nullptr) {
        FreeLibrary(discordLib);
    }

    discordLib = nullptr;
    initializeFn = nullptr;
    updateFn = nullptr;
    clearFn = nullptr;
    shutdownFn = nullptr;
    callbacksFn = nullptr;
}

void DiscordPresence::onEnable() {
    ensureInitialized();
    if (!ready) {
        logF("[DiscordPresence] Unable to start Discord Rich Presence.");
        setEnabled(false);
        return;
    }
    lastPresenceUpdate = std::chrono::steady_clock::now() - std::chrono::seconds(5);
    updatePresence();
}

void DiscordPresence::onDisable() {
    shutdownPresence();
}

void DiscordPresence::onTick(GameMode*) {
    if (!ready)
        return;

    if (callbacksFn)
        callbacksFn();

    auto now = std::chrono::steady_clock::now();
    if (now - lastPresenceUpdate >= std::chrono::seconds(5)) {
        updatePresence();
        lastPresenceUpdate = now;
    }
}
