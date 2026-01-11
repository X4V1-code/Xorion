#include "AntiArrow.h"
#include "../../../Memory/GameData.h"
#include "../../../SDK/LocalPlayer.h"

AntiArrow::AntiArrow() : IModule(0x0, Category::COMBAT, "Stops arrows within a 3 block radius") {
	registerFloatSetting("range", &range, range, 1.f, 6.f);
}

AntiArrow::~AntiArrow() {
}

const char* AntiArrow::getModuleName() {
	return "AntiArrow";
}

void AntiArrow::onTick(GameMode* gm) {
	if (!g_Data.isInGame())
		return;

	LocalPlayer* lp = g_Data.getLocalPlayer();
	if (lp == nullptr)
		return;

	Entity* lpEntity = reinterpret_cast<Entity*>(lp);
	Vec3* lpPos = lp->getPos();

	g_Data.forEachEntity([&](Entity* ent, bool) {
		if (ent == nullptr || ent == lpEntity)
			return;

		if (ent->getEntityTypeId() != 80) // Arrow entity id
			return;

		if (ent->getPos()->dist(*lpPos) <= range) {
			ent->despawn();
		}
	});
}
