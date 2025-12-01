#pragma once
#include "Module.h"
#include <string>

// Forward declaration
struct TextHolder;

class Unbanner : public IModule {
public:
    // Settings
    bool spoofName = true;       // Enable username spoofing
    bool showButton = true;      // Show UI button

    // State
    std::string spoofedUsername; // The spoofed username (word + 3 numbers)
    bool usernameGenerated = false;
    TextHolder* fakeNameHolder = nullptr; // Holds the TextHolder for GameData integration

    Unbanner();
    ~Unbanner();

    virtual const char* getModuleName() override;

    // Hooks
    void onEnable() override;
    void onDisable() override;
    void onTick(GameMode* gm) override;
    void onPostRender(MinecraftUIRenderContext* ctx) override;
    void onSendPacket(Packet* packet) override;

    // Generate a new random spoofed username
    void generateSpoofedUsername();

    // Get the current spoofed username
    const std::string& getSpoofedUsername() const;

private:
    // Render the "Try Unban Me" button
    void renderUnbanButton(MinecraftUIRenderContext* ctx);
    
    // Random word list for username generation
    static const char* wordList[];
    static const int wordListSize;
};
