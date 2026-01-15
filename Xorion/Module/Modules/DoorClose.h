#pragma once

#include "Module.h"

class DoorClose : public IModule {
private:
    int radius = 6;

public:
    DoorClose();
    ~DoorClose();

    virtual const char* getModuleName() override;
    virtual void onTick(GameMode* gm) override;
};
