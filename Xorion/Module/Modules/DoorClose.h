#pragma once

#include <unordered_set>

#include "Module.h"

class DoorClose : public IModule {
private:
    int radius = 6;
    int verticalRangeSetting = 2;
    std::unordered_set<short> doorIds{};
    std::unordered_set<short> nonDoorIds{};

public:
    DoorClose();
    ~DoorClose();

    virtual const char* getModuleName() override;
    virtual void onDisable() override;
    virtual void onTick(C_GameMode* gm) override;
};
