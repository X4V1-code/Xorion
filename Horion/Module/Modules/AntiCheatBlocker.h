#pragma once
#include "Module.h"
#include <string>
#include <vector>
#include <chrono>

// Forward declarations
struct Packet;
struct TextPacket;
struct CommandRequestPacket;

class AntiCheatBlocker : public IModule {
public:
    AntiCheatBlocker();
    ~AntiCheatBlocker();

    virtual const char* getModuleName() override;

    // Hooks
    void onTick(GameMode* gm) override;
    void onSendPacket(Packet* packet) override;

    // Override to run even when "disabled" (always-on background protection)
    bool callWhenDisabled() override { return true; }

    // Check if anti-cheat was detected
    bool wasAntiCheatDetected() const { return detectionTriggered; }

private:
    // List of known anti-cheat plugin names/patterns
    static const std::vector<std::string> knownAntiCheats;
    
    // List of known ban command patterns to intercept
    static const std::vector<std::string> banCommandPatterns;

    // Detection state
    bool detectionTriggered = false;
    bool hasKickedPlayer = false;
    std::chrono::steady_clock::time_point lastCheckTime;

    // Detection methods
    bool detectAntiCheatInMessage(const std::string& message);
    bool detectBanCommand(const std::string& command);
    
    // Protection actions
    void triggerSafeDisconnect();
    void interceptBanCommand(Packet* packet);
    
    // Utility
    std::string toLower(const std::string& str);
};
