#include "Level.h"

#include "../Memory/GameData.h"

bool Level::hasEntity() {
	return this->hitResult.type == HitResultType::Entity;
}

Entity* Level::getEntity() {
	Entity *retval = nullptr;

	if (!this->hasEntity())
		return nullptr;

	Game.forEachEntity([this, &retval](Entity *ent, bool b) {
		if (ent->ctx.entity == this->hitResult.entity.entity) {
			retval = ent;
			return;
		}
	});

	return retval;
}
