#pragma once

#include "Module.h"
#include <windows.h>
#include <chrono>
#include <cstdint>
#include <string>

class DiscordPresence : public IModule {
private:
    struct DiscordUser {
        const char* userId;
        const char* username;
        const char* discriminator;
        const char* avatar;
    };

    struct DiscordEventHandlers {
        void (*ready)(const DiscordUser*) = nullptr;
        void (*disconnected)(int errorCode, const char* message) = nullptr;
        void (*errored)(int errorCode, const char* message) = nullptr;
        void (*joinGame)(const char* joinSecret) = nullptr;
        void (*spectateGame)(const char* spectateSecret) = nullptr;
        void (*joinRequest)(const DiscordUser* request) = nullptr;
    };

    struct DiscordRichPresence {
        const char* state = nullptr;
        const char* details = nullptr;
        int64_t startTimestamp = 0;
        int64_t endTimestamp = 0;
        const char* largeImageKey = nullptr;
        const char* largeImageText = nullptr;
        const char* smallImageKey = nullptr;
        const char* smallImageText = nullptr;
        const char* partyId = nullptr;
        int partySize = 0;
        int partyMax = 0;
        const char* matchSecret = nullptr;
        const char* joinSecret = nullptr;
        const char* spectateSecret = nullptr;
        int8_t instance = 0;
    };

    using DiscordInitializeFn = void(__cdecl*)(const char*, DiscordEventHandlers*, int, const char*);
    using DiscordUpdatePresenceFn = void(__cdecl*)(const DiscordRichPresence*);
    using DiscordClearPresenceFn = void(__cdecl*)();
    using DiscordShutdownFn = void(__cdecl*)();
    using DiscordRunCallbacksFn = void(__cdecl*)();

    HMODULE discordLib = nullptr;
    DiscordInitializeFn initializeFn = nullptr;
    DiscordUpdatePresenceFn updateFn = nullptr;
    DiscordClearPresenceFn clearFn = nullptr;
    DiscordShutdownFn shutdownFn = nullptr;
    DiscordRunCallbacksFn callbacksFn = nullptr;

    bool ready = false;
    std::chrono::steady_clock::time_point lastPresenceUpdate{};
    std::int64_t startTimestamp = 0;
    std::string lastDetails;
    std::string lastState;
    bool warnedFallbackId = false;

    void loadLibrary();
    void ensureInitialized();
    void updatePresence();
    void clearPresence();
    void shutdownPresence();
    std::string resolveAppId() const;

public:
    DiscordPresence();
    ~DiscordPresence() override;

    const char* getModuleName() override;
    void onEnable() override;
    void onDisable() override;
    void onTick(GameMode* gm) override;
};
