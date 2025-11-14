// PlayerSupplies.h
#pragma once
#include "ItemStack.h"

class PlayerSupplies {
public:
    explicit PlayerSupplies(void* mcSuppliesPtr = nullptr)
        : suppliesPtr(mcSuppliesPtr) {}
    ~PlayerSupplies() = default;

    inline bool isValid() const { return suppliesPtr != nullptr; }
    inline void* getSuppliesPtr() const { return suppliesPtr; }

    int        getSelectedHotbarSlot() const;
    void       setSelectedHotbarSlot(int slot);
    ItemStack  getSelectedItem() const;
    ItemStack  getItemStackAt(int slot) const;

private:
    void* suppliesPtr;

    static int   (*PlayerSupplies_getSelectedSlot_fn)(void* supplies);
    static void  (*PlayerSupplies_setSelectedSlot_fn)(void* supplies, int slot);
    static void* (*PlayerSupplies_getItemStack_fn)(void* supplies, int slot);

    void ensureIntegration() const;
};
