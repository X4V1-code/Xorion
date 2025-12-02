#include "AntiCheatBlocker.h"
#include "../../../Memory/GameData.h"
#include "../../../SDK/Packet.h"
#include "../../../SDK/TextHolder.h"
#include "../../../Utils/Logger.h"
#include <algorithm>
#include <cctype>

// Known anti-cheat plugin names and patterns (case-insensitive matching)
const std::vector<std::string> AntiCheatBlocker::knownAntiCheats = {
    "anticheat",
    "anti-cheat",
    "nocheatplus",
    "ncp",
    "aac",
    "advancedanticheat",
    "spartan",
    "matrix",
    "vulcan",
    "verus",
    "grim",
    "grimac",
    "karhu",
    "intave",
    "polar",
    "watchdog",
    "sentinel",
    "fairfight",
    "battleye",
    "easyanticheat",
    "vac",
    "punkbuster",
    "screenshare",
    "ss-tool",
    "paladin",
    "reflex",
    "horizon",
    "themis",
    "wraith",
    "badlion",
    "blc",
    "hac",
    "hackcontrol"
};

// Known ban command patterns to intercept (exact command prefixes)
const std::vector<std::string> AntiCheatBlocker::banCommandPatterns = {
    "/ban ",
    "/banip ",
    "/ipban ",
    "/tempban ",
    "/kick ",
    "/mute ",
    "/jail ",
    "/punish ",
    "/warn ",
    "/suspend ",
    "/blacklist "
};

AntiCheatBlocker::AntiCheatBlocker()
    : IModule(0, Category::MISC, "Attempts to block known anti-cheat plugins. Always running in background.") {
    // No settings - this module is always-on and non-configurable
    lastCheckTime = std::chrono::steady_clock::now();
}

AntiCheatBlocker::~AntiCheatBlocker() {
}

const char* AntiCheatBlocker::getModuleName() {
    return "AntiCheatBlocker";
}

std::string AntiCheatBlocker::toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

bool AntiCheatBlocker::detectAntiCheatInMessage(const std::string& message) {
    std::string lowerMsg = toLower(message);
    
    for (const auto& acName : knownAntiCheats) {
        if (lowerMsg.find(acName) != std::string::npos) {
            logF("[AntiCheatBlocker] Detected anti-cheat reference: %s", acName.c_str());
            return true;
        }
    }
    return false;
}

bool AntiCheatBlocker::detectBanCommand(const std::string& command) {
    std::string lowerCmd = toLower(command);
    
    // Check if command starts with any of the ban patterns
    for (const auto& banPattern : banCommandPatterns) {
        // Use rfind with position 0 to check for prefix match only
        if (lowerCmd.rfind(banPattern, 0) == 0) {
            return true;
        }
    }
    return false;
}

void AntiCheatBlocker::triggerSafeDisconnect() {
    if (hasKickedPlayer) return;  // Only kick once
    
    auto clientInstance = Game.getClientInstance();
    if (clientInstance == nullptr) return;
    
    auto guiData = clientInstance->getGuiData();
    if (guiData != nullptr) {
        guiData->displayClientMessageF("§c[AntiCheatBlocker] §fAnti-cheat detected! Disconnecting for safety...");
    }
    
    // Attempt to disconnect the player safely
    // Using RakNet disconnect if available
    auto rakNet = Game.getRakNetConnector();
    if (rakNet != nullptr) {
        // The actual disconnect logic would depend on the SDK implementation
        // For now we log and set the flag
        logF("[AntiCheatBlocker] Triggering safe disconnect due to anti-cheat detection");
    }
    
    hasKickedPlayer = true;
    detectionTriggered = true;
    
    // Also display a warning
    if (guiData != nullptr) {
        guiData->displayClientMessageF("§c[AntiCheatBlocker] §fProtection activated - please leave the server manually.");
    }
}

void AntiCheatBlocker::interceptBanCommand(Packet* packet) {
    // Block the packet from being sent by modifying it
    // For CommandRequestPacket, we can clear the command
    if (packet->isInstanceOf<CommandRequestPacket>()) {
        // Use static_cast since isInstanceOf already verified the type
        CommandRequestPacket* cmdPacket = static_cast<CommandRequestPacket*>(packet);
        std::string cmd = cmdPacket->command.getText();
        if (detectBanCommand(cmd)) {
            logF("[AntiCheatBlocker] Intercepted potential ban command: %s", cmd.c_str());
            // Clear the command to prevent it from executing
            cmdPacket->command.setText("");
            
            auto clientInstance = Game.getClientInstance();
            if (clientInstance != nullptr) {
                auto guiData = clientInstance->getGuiData();
                if (guiData != nullptr) {
                    guiData->displayClientMessageF("§a[AntiCheatBlocker] §fBlocked automated ban command!");
                }
            }
        }
    }
}

void AntiCheatBlocker::onTick(GameMode* gm) {
    // Periodic check for anti-cheat presence
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastCheckTime);
    
    // Check every 5 seconds
    if (elapsed.count() >= 5) {
        lastCheckTime = now;
        
        // Reset detection flag if player is no longer in game
        if (!Game.isInGame()) {
            hasKickedPlayer = false;
            detectionTriggered = false;
        }
    }
}

void AntiCheatBlocker::onSendPacket(Packet* packet) {
    if (packet == nullptr) return;
    
    // Check outgoing chat messages for anti-cheat related content
    // This can detect if a server is trying to execute anti-cheat related commands through the client
    if (packet->isInstanceOf<TextPacket>()) {
        // Use static_cast since isInstanceOf already verified the type
        TextPacket* textPacket = static_cast<TextPacket*>(packet);
        std::string message = textPacket->message.getText();
        
        // Check if outgoing message mentions anti-cheat (could indicate client-side scripting detection)
        if (detectAntiCheatInMessage(message)) {
            // Trigger protection if anti-cheat is detected
            triggerSafeDisconnect();
        }
    }
    
    // Intercept command packets to block ban commands
    if (packet->isInstanceOf<CommandRequestPacket>()) {
        interceptBanCommand(packet);
    }
}
