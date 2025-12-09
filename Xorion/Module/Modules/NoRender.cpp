#include "NoRender.h"

NoRender::NoRender() : IModule(0, Category::VISUAL, "Hides particles, totem pop overlays, block suffocation overlay, and allows clear vision in water/lava.") {
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
