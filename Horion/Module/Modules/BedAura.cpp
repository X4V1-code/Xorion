// Horion/Module/Modules/BedAura.cpp
#include "BedAura.h"

#include "../../Memory/GameData.h"
#include "../../SDK/CGameMode.h"
#include "../../SDK/CClientInstance.h"
#include "../../SDK/CEntity.h"
#include "../../SDK/CInventory.h"
#include "../../SDK/CItem.h"
#include "../../SDK/CBlockLegacy.h"
#include "../../SDK/CMinecraftUIRenderContext.h"
#include "../../Utils/DrawUtils.h"
#include "../../Utils/Logger.h"

static inline bool isBedBlockLegacy(CBlockLegacy* legacy) {
    if (!legacy) return false;
    // Safer than numeric IDs: Bed colors vary by ID but share the same legacy block name.
    // Expected names contain "bed" (e.g., "minecraft:white_bed", "minecraft:red_bed").
    auto name = legacy->getName().getText();
    return name.find("bed") != std::string::npos;
}

static inline bool isBedItem(CItem* item) {
    if (!item) return false;
    // Item names for beds also contain "bed" across colors.
    auto name = item->name.getText();
    return name.find("bed") != std::string::npos;
}

BedAura::BedAura()
    : IModule(VK_NUMPAD2, Category::COMBAT, "Auto place and detonate beds in Nether/End.") {
    // Settings
    registerIntSetting("Range", &range, range, 2, 10);
    registerBoolSetting("Autoplace", &autoplace, autoplace);
    registerBoolSetting("AutoDetonateNearby", &autoDetonateNearby, autoDetonateNearby);
    registerBoolSetting("DimensionGuard", &dimensionGuard, dimensionGuard);
    registerIntSetting("DelayTicks", &cycleDelay, cycleDelay, 2, 20);

    // Visuals
    registerBoolSetting("RenderBeds", &renderBeds, renderBeds);
    registerFloatSetting("RenderAlpha", &renderAlpha, renderAlpha, 0.1f, 1.0f);

    // State
    delay = 0;
    prevSlot = -1;
    restoreSlotPending = false;
}

BedAura::~BedAura() {}

const char* BedAura::getModuleName() {
    return "BedAura";
}

void BedAura::onEnable() {
    delay = 0;
    prevSlot = -1;
    restoreSlotPending = false;
    bedPositions.clear();
}

void BedAura::onDisable() {
    delay = 0;
    if (restoreSlotPending) {
        auto lp = Game.getLocalPlayer();
        if (lp) {
            auto supplies = lp->getSupplies();
            if (supplies && prevSlot >= 0) supplies->selectedHotbarSlot = prevSlot;
        }
    }
    prevSlot = -1;
    restoreSlotPending = false;
    bedPositions.clear();
}

void BedAura::onTick(C_GameMode* gm) {
    auto lp = Game.getLocalPlayer();
    if (!lp || !gm) return;
    if (!lp->region || !lp->level) return;

    // Dimension guard: Only detonate/place in Nether/End.
    // TODO: Replace dimension check with your SDK's canonical dimension IDs.
    // Common mapping: 0=Overworld, 1=Nether, 2=End.
    int dimId = lp->level->dimension; // TODO: confirm actual field
    if (dimensionGuard && dimId == 0) {
        // Overworld: using beds attempts to sleep; skip to stay combat-safe.
        bedPositions.clear();
        return;
    }

    delay++;
    bedPositions.clear();

    const vec3_t playerPos = *lp->getPos();
    vec3_ti basePos((int)floor(playerPos.x), (int)floor(playerPos.y), (int)floor(playerPos.z));

    // 1) Scan for nearby existing beds and queue them for detonation
    if (autoDetonateNearby) {
        for (int x = -range; x <= range; x++) {
            for (int y = -2; y <= 2; y++) {
                for (int z = -range; z <= range; z++) {
                    vec3_ti pos = basePos.add(x, y, z);
                    auto block = lp->region->getBlock(pos);
                    if (!block) continue;

                    auto legacy = block->toLegacy();
                    if (!legacy) continue;

                    if (isBedBlockLegacy(legacy)) {
                        bedPositions.push_back(pos);
                    }
                }
            }
        }

        // Detonate the beds we found by "using" them
        for (auto& bpos : bedPositions) {
            // Facing and hit vec are heuristic; server logic will still treat it as an interact
            // in Nether/End, causing explosion.
            Vec3i blockPos(bpos.x, bpos.y, bpos.z);
            uint8_t facing = 1; // TODO: optionally use actual face from ray trace
            vec3_t hit(0.5f, 0.5f, 0.5f);

            // Use currently selected item (doesn't need to be a bed to detonate an existing one)
            auto supplies = lp->getSupplies();
            C_ItemStack* selected = nullptr;
            if (supplies && supplies->inventory) {
                selected = supplies->inventory->getItemStack(supplies->selectedHotbarSlot);
            }

            // gm->useItemOn can be used without a bed in hand to interact with an existing bed block.
            // TODO: Confirm your GameMode API signature and call.
            gm->useItemOn(selected, blockPos, facing, hit); // TODO: adjust signature if needed
        }
    }

    // 2) Autoplace: find a bed in hotbar, place it in front, then detonate it
    if (autoplace && delay % cycleDelay == 0) {
        auto supplies = lp->getSupplies();
        if (!supplies || !supplies->inventory) return;

        // Save current slot
        prevSlot = supplies->selectedHotbarSlot;
        restoreSlotPending = true;

        int bedSlot = -1;
        // Scan hotbar for any bed item (color-agnostic)
        for (int slot = 0; slot < 9; slot++) {
            auto stack = supplies->inventory->getItemStack(slot);
            if (!stack || !stack->item) continue;
            if (isBedItem(stack->getItem())) {
                bedSlot = slot;
                break;
            }
        }

        if (bedSlot != -1) {
            supplies->selectedHotbarSlot = bedSlot;

            // Compute a place position: one block ahead, floor-aligned
            vec3_t forward = lp->viewAngles.toDirection(); // TODO: confirm you have a helper for view direction
            vec3_t placePosF = playerPos.add(forward).floor();
            Vec3i placePos((int)placePosF.x, (int)placePosF.y, (int)placePosF.z);

            // Use block face from current hit result when available
            uint8_t face = 1;
            if (lp->level->hitResult.type == 0 /* HitResultType::Tile */) {
                face = (uint8_t)lp->level->hitResult.facing;
            }

            bool useBlockSide = true;
            // Place the bed block
            gm->buildBlock(&placePos, face, useBlockSide); // TODO: ensure this matches your buildBlock signature

            // Detonate right after placement (Nether/End -> explode)
            vec3_t hit(0.5f, 0.5f, 0.5f);
            auto stack = supplies->inventory->getItemStack(bedSlot);
            gm->useItemOn(stack, placePos, face, hit); // TODO: confirm API signature
        }

        // Restore slot a bit later to avoid flicker during place/use
        slotRestoreCountdown = 2; // ticks until restore
    }

    // 3) Restore previous slot after actions
    if (restoreSlotPending) {
        if (slotRestoreCountdown > 0) {
            slotRestoreCountdown--;
        } else {
            auto supplies = lp->getSupplies();
            if (supplies && prevSlot >= 0) {
                supplies->selectedHotbarSlot = prevSlot;
            }
            restoreSlotPending = false;
            prevSlot = -1;
        }
    }
}

void BedAura::onLevelRender(C_MinecraftUIRenderContext* ctx) {
    if (!renderBeds) return;
    if (bedPositions.empty()) return;

    DrawUtils::setColor(0.9f, 0.3f, 0.3f, renderAlpha);
    for (auto& p : bedPositions) {
        vec3_t start((float)p.x, (float)p.y, (float)p.z);
        vec3_t end(start.x + 1.f, start.y + 1.f, start.z + 1.f);
        DrawUtils::drawBox(start, end, 0.4f);
    }
}
