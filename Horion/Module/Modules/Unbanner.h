#pragma once
#include "Module.h"
#include <string>

class Unbanner : public IModule {
public:
    // Example settings
    bool toggleOption = true;
    int intOption = 5;
    float floatOption = 0.5f;
    std::string stringOption = "Default";

    // State
    int tickCounter = 0;

    Unbanner();
    ~Unbanner();

    virtual const char* getModuleName() override;

    // Hooks
    void onEnable() override;
    void onDisable() override;
    void onTick(GameMode* gm) override;
    // TODO: onLevelRender and onRenderGUI not in IModule base
    // void onLevelRender(C_MinecraftUIRenderContext* ctx) override;
    // void onRenderGUI(C_MinecraftUIRenderContext* ctx) override;
};
