#include "NoRender.h"

NoRender::NoRender() : IModule(0, Category::VISUAL, "Removes water/lava fog for clear vision. Additional options for particles, totem pop, and block overlay available.") {
	// Note: Particles, Totem Pop, and Block Overlay settings are available but may require
	// additional hooks to be fully functional. Water/Lava fog removal is fully implemented.
	registerBoolSetting("Particles", &particles, particles);
	registerBoolSetting("Totem Pop", &totemPop, totemPop);
	registerBoolSetting("Water Fog", &waterFog, waterFog);
	registerBoolSetting("Lava Fog", &lavaFog, lavaFog);
	registerBoolSetting("Block Overlay", &blockOverlay, blockOverlay);
}

NoRender::~NoRender() {}

const char* NoRender::getModuleName() {
	return "NoRender";
}
