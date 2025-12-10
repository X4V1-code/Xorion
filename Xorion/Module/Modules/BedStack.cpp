#include "BedStack.h"
#include "../../../Memory/GameData.h"
#include "../../../SDK/GameMode.h"
#include "../../../SDK/ItemStack.h"
#include "../../../SDK/Inventory.h"

// Inventory size constants
constexpr int TOTAL_INVENTORY_SLOTS = 36; // Hotbar (0-8) + Main inventory (9-35)

// Cache for bed item detection to avoid string operations every tick
static inline bool isBedItem(Item* item) {
	if (!item) return false;
	
	// Check by item ID if known (beds are typically 355-370 depending on color)
	// This is more efficient than string matching
	int itemId = item->itemId;
	if (itemId >= 355 && itemId <= 370) {
		return true;
	}
	
	// Fallback to string matching for mod beds or unknown IDs
	std::string name = item->name.getText();
	return name.find("bed") != std::string::npos;
}

BedStack::BedStack() : IModule(0, Category::PLAYER, "Allows same-color beds to stack up to 64.") {
	registerIntSetting("MaxStackSize", &maxStackSize, maxStackSize, 1, 64);
}

BedStack::~BedStack() {
}

const char* BedStack::getModuleName() {
	return "BedStack";
}

void BedStack::onTick(C_GameMode* gm) {
	if (!g_Data.getLocalPlayer())
		return;

	auto player = g_Data.getLocalPlayer();
	auto supplies = player->getSupplies();
	if (!supplies || !supplies->inventory)
		return;

	auto inv = supplies->inventory;
	
	// Iterate through all inventory slots
	for (int i = 0; i < TOTAL_INVENTORY_SLOTS; i++) {
		ItemStack* stack = inv->getByGlobalIndex(i);
		if (!stack || !stack->item)
			continue;

		// Check if this is a bed item
		if (isBedItem(stack->item)) {
			// Set the max stack size to allow stacking
			stack->item->setMaxStackSize(static_cast<uint8_t>(maxStackSize));
			stack->item->setStackedByData(true);
		}
	}
}
