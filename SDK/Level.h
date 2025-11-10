#pragma once

#include <iostream>
#include <string>

#include "Entity.h"
#include "HitResult.h"
#include "../Utils/HMath.h"
#include "../Utils/Utils.h"

class Level {
public:
	BUILD_ACCESS(this, int, levelTicks, 0x8A0);
	BUILD_ACCESS(this, HitResult, hitResult, 0xBD8);
	BUILD_ACCESS(this, HitResult, liquidHitResult, 0xC48);

public:
	bool hasEntity();
	Entity *getEntity();

	int getLevelTicks() {
		return levelTicks;
	}

	void playSound(std::string const& sound, Vec3 const &position, float volume, float pitch) {
		Utils::CallVFunc<170, void>(this, sound, position, volume, pitch);
	}

	std::vector<Entity *> getMiscEntityList() {  // Level::getRuntimeActorList
		return Utils::CallVFunc<304, std::vector<Entity *>>(this);
	}
};
