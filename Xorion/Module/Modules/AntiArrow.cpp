#include "AntiArrow.h"
#include "../../../Memory/GameData.h"
#include "../../../SDK/LocalPlayer.h"
#include "../../../SDK/EntityTypeIds.h"

AntiArrow::AntiArrow() : IModule(0x0, Category::COMBAT, "Stops arrows around you") {
	registerFloatSetting("range", &range, range, 1.f, 6.f);
}

AntiArrow::~AntiArrow() {
}

const char* AntiArrow::getModuleName() {
	return "AntiArrow";
}

void AntiArrow::onTick(C_GameMode* gm) {
	if (!g_Data.isInGame())
		return;

	LocalPlayer* lp = g_Data.getLocalPlayer();
	if (lp == nullptr)
		return;

	Vec3* lpPos = lp->getPos();
	if (lpPos == nullptr)
		return;

	g_Data.forEachEntity([&](Entity* ent, bool) {
		if (ent == nullptr)
			return;

		if (ent->getEntityTypeId() != static_cast<int>(EntityTypeId::Arrow))
			return;

		Vec3* entPos = ent->getPos();
		if (entPos == nullptr)
			return;

		if (entPos->dist(*lpPos) <= range) {
			ent->despawn();
		}
	});
}
