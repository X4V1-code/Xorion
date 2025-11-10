#pragma once
#include "Module.h"
#include <vector>

class BedAura : public IModule {
public:
    // Settings
    int range = 5;
    bool autoplace = true;
    bool autoDetonateNearby = true;
    bool dimensionGuard = true;
    int cycleDelay = 5;

    bool renderBeds = true;
    float renderAlpha = 0.4f;

    // State
    int delay = 0;
    int prevSlot = -1;
    bool restoreSlotPending = false;
    int slotRestoreCountdown = 0;

    std::vector<vec3_ti> bedPositions;

    BedAura();
    ~BedAura();

    virtual const char* getModuleName() override;

    // Hooks
    void onEnable() override;
    void onDisable() override;
    void onTick(C_GameMode* gm) override;
    void onLevelRender(C_MinecraftUIRenderContext* ctx) override;
};
