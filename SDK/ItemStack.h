// ItemStack.h
#pragma once
#include <string>

class ItemStack {
public:
    explicit ItemStack(void* mcItemStackPtr = nullptr);
    ~ItemStack();

    bool isNull() const { return itemPtr == nullptr; }
    void* getItemPtr() const { return itemPtr; }

    int getCount() const;
    void setCount(int newCount);
    std::string getItemName() const;

private:
    void* itemPtr;

    static int   (*ItemStack_getCount_fn)(void* itemStack);
    static void  (*ItemStack_setCount_fn)(void* itemStack, int count);
    static const char* (*ItemStack_getName_fn)(void* itemStack);

    void ensureIntegration() const;
};
