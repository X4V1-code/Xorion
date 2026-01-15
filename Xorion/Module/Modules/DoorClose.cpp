#include "DoorClose.h"
#include "../../../Memory/GameData.h"
#include "../../../SDK/BlockLegacy.h"
#include "../../../SDK/GameMode.h"
#include "../../../SDK/HitResult.h"

#include <algorithm>
#include <cctype>
#include <string_view>
#include <unordered_set>

DoorClose::DoorClose() : IModule(0, Category::WORLD, "Automatically closes nearby doors and keeps them shut.") {
    registerIntSetting("Radius", &radius, radius, 1, 10);
    registerIntSetting("Height", &verticalRangeSetting, verticalRangeSetting, 1, 6);
}

DoorClose::~DoorClose() {
}

const char* DoorClose::getModuleName() {
    return "DoorClose";
}

void DoorClose::onDisable() {
    doorIds.clear();
    nonDoorIds.clear();
}

void DoorClose::onTick(C_GameMode* gm) {
    auto lp = Game.getLocalPlayer();
    if (!gm || !lp || !lp->getRegion())
        return;

    Vec3* pos = lp->getPos();
    if (!pos)
        return;

    const int startX = (int)pos->x - radius;
    const int endX = (int)pos->x + radius;
    // Limit vertical scan to the configured height but never exceed the horizontal search radius
    const int verticalRange = std::min(radius, verticalRangeSetting);
    const int startY = (int)pos->y - verticalRange;
    const int endY = (int)pos->y + verticalRange;
    const int startZ = (int)pos->z - radius;
    const int endZ = (int)pos->z + radius;

    // Door block state bits
    constexpr uint8_t DOOR_OPEN_BIT = 0x4;   // open_bit flag in block data
    constexpr uint8_t DOOR_UPPER_BIT = 0x8;  // upper_block_bit flag in block data

    auto isDoorBlock = [&doorIds = this->doorIds, &nonDoorIds = this->nonDoorIds](BlockLegacy* legacy) {
        if (!legacy)
            return false;

        short blockId = legacy->blockId;

        if (doorIds.find(blockId) != doorIds.end())
            return true;

        if (nonDoorIds.find(blockId) != nonDoorIds.end())
            return false;

        auto nameHasDoor = [](const TextHolder& holder) {
            std::string_view text(holder.getText(), holder.getTextLength());
            constexpr std::string_view needle = "door";
            auto it = std::search(text.begin(), text.end(), needle.begin(), needle.end(),
                                  [](char a, char b) {
                                      return std::tolower(static_cast<unsigned char>(a)) ==
                                             std::tolower(static_cast<unsigned char>(b));
                                  });
            return it != text.end();
        };

        if (nameHasDoor(legacy->getName()) || nameHasDoor(legacy->tileName)) {
            doorIds.insert(blockId);
            return true;
        }

        nonDoorIds.insert(blockId);
        return false;
    };

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
                if (!isDoorBlock(legacy))
                    continue;

                constexpr uint8_t DEFAULT_INTERACT_FACE = 1;  // Face index 1 = top; used when no hitResult is available
                constexpr bool USE_BLOCK_SIDE = true;

                // buildBlock triggers the door toggle interaction when useBlockSide is true
                gm->buildBlock(&blockPos, DEFAULT_INTERACT_FACE, USE_BLOCK_SIDE);
                lp->swingArm();
            }
        }
    }
}
