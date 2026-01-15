#include "DoorClose.h"
#include "../../../Memory/GameData.h"
#include "../../../SDK/BlockLegacy.h"
#include "../../../SDK/GameMode.h"

#include <algorithm>
#include <unordered_set>

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

    constexpr int DOOR_VERTICAL_RANGE = 2;
    const int startX = (int)pos->x - radius;
    const int endX = (int)pos->x + radius;
    const int verticalRange = std::min(radius, DOOR_VERTICAL_RANGE);
    const int startY = (int)pos->y - verticalRange;
    const int endY = (int)pos->y + verticalRange;
    const int startZ = (int)pos->z - radius;
    const int endZ = (int)pos->z + radius;

    // Door block state bits
    constexpr uint8_t DOOR_OPEN_BIT = 0x4;   // open_bit flag in block data
    constexpr uint8_t DOOR_UPPER_BIT = 0x8;  // upper_block_bit flag in block data

    for (int x = startX; x <= endX; x++) {
        for (int y = startY; y <= endY; y++) {
            for (int z = startZ; z <= endZ; z++) {
                Vec3i blockPos(x, y, z);
                Block* block = lp->getRegion()->getBlock(blockPos);
                if (!block)
                    continue;

                uint8_t data = block->data;
                if ((data & DOOR_OPEN_BIT) == 0)
                    continue;  // already closed

                if (data & DOOR_UPPER_BIT)
                    continue;  // operate on the lower half only to avoid double toggles

                BlockLegacy* legacy = block->toLegacy();
                if (!legacy)
                    continue;

                short blockId = legacy->blockId;
                static std::unordered_set<short> doorIds;
                static std::unordered_set<short> nonDoorIds;

                if (doorIds.find(blockId) == doorIds.end()) {
                    if (nonDoorIds.find(blockId) != nonDoorIds.end())
                        continue;

                    std::string name = legacy->getName().getText();
                    if (name.find("door") == std::string::npos) {
                        nonDoorIds.insert(blockId);
                        continue;
                    }

                    doorIds.insert(blockId);
                }

                constexpr uint8_t INTERACT_FACE = 0;
                bool useBlockSide = true;
                gm->buildBlock(&blockPos, INTERACT_FACE, useBlockSide);
                lp->swingArm();
            }
        }
    }
}
