#pragma once

#include "Module.h"

class NoRender : public IModule {
private:
	bool particles = true;
	bool totemPop = true;
	bool waterFog = true;
	bool lavaFog = true;
	bool blockOverlay = true;

public:
	NoRender();
	~NoRender();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	
	// Getters for hook access
	bool shouldHideParticles() const { return particles && isEnabled(); }
	bool shouldHideTotemPop() const { return totemPop && isEnabled(); }
	bool shouldHideWaterFog() const { return waterFog && isEnabled(); }
	bool shouldHideLavaFog() const { return lavaFog && isEnabled(); }
	bool shouldHideBlockOverlay() const { return blockOverlay && isEnabled(); }
};
