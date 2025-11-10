#pragma once
#include "ItemStack.h"

/*
    SDK Wrapper: PlayerSupplies
    - Thin wrapper around Minecraft's PlayerSupplies struct.
    - Requires SlimMem signature resolution at startup.
    - Calls into Minecraft vtables; no local state.
    - Must be re-mapped each Minecraft version.
*/

class PlayerSupplies {
public:
    explicit PlayerSupplies(void* mcSuppliesPtr = nullptr)
        : suppliesPtr(mcSuppliesPtr) {}

    ~PlayerSupplies() = default;

    inline bool isValid() const { return suppliesPtr != nullptr; }
    inline void* getSuppliesPtr() const { return suppliesPtr; }

    int getSelectedHotbarSlot() const;
    void setSelectedHotbarSlot(int slot);

    ItemStack getSelectedItem() const;
    ItemStack getItemStackAt(int slot) const;

private:
    void* suppliesPtr;

    static int   (*PlayerSupplies_getSelectedSlot_fn)(void* supplies);
    static void  (*PlayerSupplies_setSelectedSlot_fn)(void* supplies, int slot);
    static void* (*PlayerSupplies_getItemStack_fn)(void* supplies, int slot);

    void ensureIntegration() const;
};
