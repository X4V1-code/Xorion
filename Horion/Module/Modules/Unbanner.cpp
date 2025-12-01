#include "Unbanner.h"
#include "../../DrawUtils.h"
#include "../../../Memory/GameData.h"
#include "../../../SDK/Packet.h"
#include "../../../SDK/TextHolder.h"
#include "../../../Utils/Utils.h"
#include <random>
#include <ctime>

// Xorion green color (same as xorion-banner.png)
static const MC_Color xorionGreen = MC_Color(37, 164, 64);
static const MC_Color whiteColor = MC_Color(255, 255, 255);

// Word list for generating random usernames
const char* Unbanner::wordList[] = {
    "Player", "Gamer", "Hero", "Knight", "Dragon", "Shadow", "Storm", "Fire",
    "Ice", "Thunder", "Wolf", "Eagle", "Tiger", "Lion", "Bear", "Hawk",
    "Ninja", "Warrior", "Mage", "Ranger", "Hunter", "Scout", "Blade", "Sword",
    "Shield", "Arrow", "Spark", "Blaze", "Frost", "Wind", "Stone", "Iron",
    "Steel", "Gold", "Silver", "Bronze", "Copper", "Ruby", "Emerald", "Diamond",
    "Swift", "Quick", "Fast", "Rapid", "Sonic", "Flash", "Bolt", "Strike",
    "Mighty", "Strong", "Power", "Force", "Energy", "Spirit", "Soul", "Heart",
    "Dark", "Light", "Bright", "Glow", "Shine", "Beam", "Ray", "Star"
};
const int Unbanner::wordListSize = sizeof(wordList) / sizeof(wordList[0]);

Unbanner::Unbanner()
    : IModule(0, Category::MISC, "Spoofs your username with a random name to help avoid bans.") {
    registerBoolSetting("SpoofName", &spoofName, spoofName);
    registerBoolSetting("ShowButton", &showButton, showButton);
}

Unbanner::~Unbanner() {
    // Clean up the fake name holder if it exists
    if (fakeNameHolder != nullptr) {
        // Only clear from GameData if we're the one who set it
        if (Game.getFakeName() == fakeNameHolder) {
            Game.setFakeName(nullptr);
        }
        delete fakeNameHolder;
        fakeNameHolder = nullptr;
    }
}

const char* Unbanner::getModuleName() {
    return "Unbanner";
}

void Unbanner::generateSpoofedUsername() {
    // Seed random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Pick a random word from the list
    std::uniform_int_distribution<> wordDist(0, wordListSize - 1);
    std::string word = wordList[wordDist(gen)];
    
    // Generate 3 random digits
    std::uniform_int_distribution<> digitDist(0, 9);
    std::string numbers;
    for (int i = 0; i < 3; i++) {
        numbers += std::to_string(digitDist(gen));
    }
    
    // Combine word and numbers
    spoofedUsername = word + numbers;
    usernameGenerated = true;
    
    // Create a TextHolder for the fake name and set it in GameData
    // This integrates with the existing name spoofing system
    if (fakeNameHolder != nullptr) {
        delete fakeNameHolder;
    }
    fakeNameHolder = new TextHolder(spoofedUsername);
    Game.setFakeName(fakeNameHolder);
    
    // Log the generated username
    auto clientInstance = Game.getClientInstance();
    if (clientInstance != nullptr) {
        auto guiData = clientInstance->getGuiData();
        if (guiData != nullptr) {
            guiData->displayClientMessageF("[Unbanner] Generated spoofed username: %s", spoofedUsername.c_str());
        }
    }
}

const std::string& Unbanner::getSpoofedUsername() const {
    return spoofedUsername;
}

void Unbanner::onEnable() {
    // Generate a new spoofed username when the module is enabled
    generateSpoofedUsername();
    
    auto clientInstance = Game.getClientInstance();
    if (clientInstance != nullptr) {
        auto guiData = clientInstance->getGuiData();
        if (guiData != nullptr) {
            guiData->displayClientMessageF("[Unbanner] Enabled - Your spoofed name: %s", spoofedUsername.c_str());
            guiData->displayClientMessageF("[Unbanner] Reconnect to apply the spoofed name to the server.");
        }
    }
}

void Unbanner::onDisable() {
    // Clear the fake name from GameData
    Game.setFakeName(nullptr);
    if (fakeNameHolder != nullptr) {
        delete fakeNameHolder;
        fakeNameHolder = nullptr;
    }
    
    usernameGenerated = false;
    spoofedUsername.clear();
    
    auto clientInstance = Game.getClientInstance();
    if (clientInstance != nullptr) {
        auto guiData = clientInstance->getGuiData();
        if (guiData != nullptr) {
            guiData->displayClientMessageF("[Unbanner] Disabled - Username spoofing deactivated.");
        }
    }
}

void Unbanner::onTick(GameMode* gm) {
    // Ensure we have a spoofed username
    if (spoofName && !usernameGenerated) {
        generateSpoofedUsername();
    }
    
    // Keep the fake name in GameData synchronized
    if (spoofName && usernameGenerated && Game.getFakeName() == nullptr && fakeNameHolder != nullptr) {
        Game.setFakeName(fakeNameHolder);
    }
}

void Unbanner::onSendPacket(Packet* packet) {
    if (!spoofName || !usernameGenerated || spoofedUsername.empty()) {
        return;
    }
    
    LocalPlayer* localPlayer = Game.getLocalPlayer();
    if (localPlayer == nullptr) {
        return;
    }
    
    // Intercept TextPacket (chat messages) and modify the source name
    if (packet->isInstanceOf<TextPacket>()) {
        TextPacket* textPacket = reinterpret_cast<TextPacket*>(packet);
        
        // Get the original player name to compare
        TextHolder* originalName = localPlayer->getNameTag();
        if (originalName != nullptr) {
            std::string origNameStr = originalName->getText();
            std::string sourceNameStr = textPacket->sourceName.getText();
            
            // If the source name matches the local player's name, replace it
            if (sourceNameStr == origNameStr || sourceNameStr.find(origNameStr) != std::string::npos) {
                textPacket->sourceName.setText(spoofedUsername);
            }
        }
    }
}

void Unbanner::onPostRender(MinecraftUIRenderContext* ctx) {
    if (showButton) {
        renderUnbanButton(ctx);
    }
}

void Unbanner::renderUnbanButton(MinecraftUIRenderContext* ctx) {
    // Null safety checks
    auto clientInstance = Game.getClientInstance();
    if (clientInstance == nullptr) return;
    auto guiData = clientInstance->getGuiData();
    if (guiData == nullptr) return;

    // Get window size for positioning
    Vec2 windowSize = guiData->windowSize;

    // Button dimensions
    const float buttonWidth = 180.0f;
    const float buttonHeight = 35.0f;

    // Center button horizontally, position near bottom
    float buttonX = (windowSize.x - buttonWidth) / 2.0f;
    float buttonY = windowSize.y - buttonHeight - 60.0f;

    // Button rectangle
    Vec4 buttonRect(buttonX, buttonY, buttonX + buttonWidth, buttonY + buttonHeight);

    // Get mouse position with null check
    auto mousePtr = clientInstance->getMousePos();
    if (mousePtr == nullptr) return;
    Vec2 mousePos = *mousePtr;
    Vec2 windowSizeReal = guiData->windowSizeReal;
    mousePos = mousePos.div(windowSizeReal);
    mousePos = mousePos.mul(windowSize);

    // Check if mouse is hovering over button
    bool isHovered = buttonRect.contains(&mousePos);

    // Draw button background with Xorion green
    MC_Color buttonColor = isHovered ? xorionGreen.lerp(whiteColor, 0.2f) : xorionGreen;
    DrawUtils::fillRectangle(buttonRect, buttonColor, 1.0f);

    // Draw button text with current spoofed name
    std::string buttonText = usernameGenerated ? ("Name: " + spoofedUsername) : "Generate New Name";
    float textWidth = DrawUtils::getTextWidth(&buttonText, 1.0f);
    float textHeight = DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
    Vec2 textPos(
        buttonX + (buttonWidth - textWidth) / 2.0f,
        buttonY + (buttonHeight - textHeight) / 2.0f
    );
    DrawUtils::drawText(textPos, &buttonText, whiteColor, 1.0f, 1.0f);

    // Handle click - regenerate username
    if (isHovered && DrawUtils::shouldToggleLeftClick) {
        DrawUtils::shouldToggleLeftClick = false;
        generateSpoofedUsername();
    }
}
