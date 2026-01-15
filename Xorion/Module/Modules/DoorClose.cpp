#include "DoorClose.h"
#include "../../../Memory/GameData.h"
#include "../../../SDK/BlockLegacy.h"
#include "../../../SDK/GameMode.h"

#include <algorithm>
#include <cctype>

DoorClose::DoorClose() : IModule(0, Category::WORLD, "Automatically closes nearby doors and keeps them shut.") {
    registerIntSetting("Radius", &radius, radius, 1, 10);
}

DoorClose::~DoorClose() {
}

const char* DoorClose::getModuleName() {
    return "DoorClose";
}

void DoorClose::onTick(GameMode* gm) {
    auto lp = Game.getLocalPlayer();
    if (!gm || !lp || !lp->getRegion())
        return;

    Vec3* pos = lp->getPos();
    if (!pos)
        return;

    const int startX = (int)pos->x - radius;
    const int endX = (int)pos->x + radius;
    const int startY = (int)pos->y - radius;
    const int endY = (int)pos->y + radius;
    const int startZ = (int)pos->z - radius;
    const int endZ = (int)pos->z + radius;

    constexpr uint8_t DOOR_OPEN_BIT = 0x4;
    constexpr uint8_t DOOR_UPPER_BIT = 0x8;

    for (int x = startX; x <= endX; x++) {
        for (int y = startY; y <= endY; y++) {
            for (int z = startZ; z <= endZ; z++) {
                Vec3i blockPos(x, y, z);
                Block* block = lp->getRegion()->getBlock(blockPos);
                if (!block)
                    continue;

                BlockLegacy* legacy = block->toLegacy();
                if (!legacy)
                    continue;

                std::string name = legacy->getName().getText();
                std::transform(name.begin(), name.end(), name.begin(),
                               [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
                if (name.find("door") == std::string::npos)
                    continue;

                uint8_t data = block->data;
                if ((data & DOOR_OPEN_BIT) == 0)
                    continue;  // already closed

                if (data & DOOR_UPPER_BIT)
                    continue;  // operate on the lower half only to avoid double toggles

                bool useBlockSide = true;
                gm->buildBlock(&blockPos, 0, useBlockSide);
                lp->swingArm();
            }
        }
    }
}
