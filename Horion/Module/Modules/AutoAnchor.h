#pragma once
#include "Module.h"

class AutoAnchor : public IModule {
public:
    AutoAnchor();
    ~AutoAnchor();

    virtual const char* getModuleName() override;
    virtual void onTick(GameMode* gm) override;

private:
    int chargeStep = 0;
    bool active = false;
};
