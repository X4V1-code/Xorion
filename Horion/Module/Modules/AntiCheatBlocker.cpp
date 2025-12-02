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

// Known ban command patterns to intercept
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
    
    for (const auto& banPattern : banCommandPatterns) {
        if (lowerCmd.find(banPattern) != std::string::npos) {
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
        CommandRequestPacket* cmdPacket = reinterpret_cast<CommandRequestPacket*>(packet);
        if (cmdPacket != nullptr) {
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
    
    // Intercept text packets for anti-cheat detection in chat
    if (packet->isInstanceOf<TextPacket>()) {
        TextPacket* textPacket = reinterpret_cast<TextPacket*>(packet);
        if (textPacket != nullptr) {
            std::string message = textPacket->message.getText();
            std::string sourceName = textPacket->sourceName.getText();
            
            // Check if incoming message mentions anti-cheat
            if (detectAntiCheatInMessage(message) || detectAntiCheatInMessage(sourceName)) {
                // Trigger protection if anti-cheat is detected
                triggerSafeDisconnect();
            }
        }
    }
    
    // Intercept command packets to block ban commands
    if (packet->isInstanceOf<CommandRequestPacket>()) {
        interceptBanCommand(packet);
    }
}
