#pragma once

#include "../Utils/HMath.h"
#include "EntityRefs.h"

enum class FacingID : char {
	Unknown = -1,
	Down = 0,
	Up = 1,
	North = 2,
	South = 3,
	West = 4,
	East = 5,
};

enum class HitResultType : int {
	Tile = 0x0,
	Entity = 0x1,
	EntityOutOfRange = 0x2,
	NoHit = 0x3,
};

class HitResult {
public:
	Vec3 startPos;
	Vec3 rayDir;
	HitResultType type;
	FacingID facing;
	Vec3i blockPos;
	Vec3 pos;
	WeakEntityRef entity;
	bool isHitLiquid;
	FacingID liquidFacing;
	Vec3i liquid;
	Vec3 liquidPos;
	bool indirectHit;
};
