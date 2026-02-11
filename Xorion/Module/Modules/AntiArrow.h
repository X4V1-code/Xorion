#pragma once

#include "Module.h"

class AntiArrow : public IModule {
private:
	float range = 3.f;

public:
	AntiArrow();
	~AntiArrow();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
};
