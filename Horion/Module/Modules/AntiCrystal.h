// Horion/Module/Modules/AntiCrystal.h
#pragma once
#include "Module.h"
#include "../../SDK/C_Entity.h"

class AntiCrystal : public IModule {
private:
    float range = 5.0f;
    bool autoBreak = true;
    bool dodge = true;

    void actOnCrystals(C_GameMode* gm);

public:
    AntiCrystal();
    ~AntiCrystal();

    virtual const char* getModuleName() override;
    virtual void onTick(C_GameMode* gm) override;
};
