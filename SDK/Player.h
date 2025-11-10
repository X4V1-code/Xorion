#pragma once
#include "CEntity.h"
#include "ItemStack.h"
#include "Vec3.h"
#include <string>

/*
    SDK Wrapper: Player
    - Thin wrapper around Minecraft's Player struct.
    - Requires SlimMem signature resolution at startup.
    - Calls into Minecraft vtables; no local state.
    - Must be re-mapped each Minecraft version.
*/

class Player : public C_Entity {
public:
    explicit Player(void* mcPlayerPtr = nullptr);
    virtual ~Player();

    std::string getName() const;
    ItemStack   getHeldItem() const;
    Vec3        getPos() const;
    void        setPos(const Vec3& pos);
    float       getHealth() const;
    void        setHealth(float health);

    inline void* getMcPlayer() const { return mcPlayerPtr; }

protected:
    void* mcPlayerPtr;

    static const char* (*Player_getName_fn)(void* mcPlayer);
    static void*       (*Player_getItemStackInHand_fn)(void* mcPlayer);
    static Vec3        (*Player_getPos_fn)(void* mcPlayer);
    static void        (*Player_setPos_fn)(void* mcPlayer, const Vec3& pos);
    static float       (*Player_getHealth_fn)(void* mcPlayer);
    static void        (*Player_setHealth_fn)(void* mcPlayer, float health);

    void ensureIntegration() const;
};
