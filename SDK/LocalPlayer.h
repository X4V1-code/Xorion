#pragma once
#include "Player.h"
#include <string>

/*
    SDK Wrapper: LocalPlayer
    - Thin wrapper around Minecraft's LocalPlayer struct.
    - Requires SlimMem signature resolution at startup.
    - Calls into Minecraft vtables; no local state.
    - Must be re-mapped each Minecraft version.
*/

class C_GameMode;
class ClientInstance;

class LocalPlayer : public Player {
public:
    explicit LocalPlayer(void* mcLocalPlayerPtr = nullptr);
    ~LocalPlayer();

    void sendChatMessage(const std::string& msg);
    void attackEntity(C_Entity* entity);
    void jump();

    bool isSneaking() const;
    void setSneaking(bool sneaking);

    C_GameMode*     getGameMode() const;
    ClientInstance* getClientInstance() const;

private:
    bool sneaking;

    static void (*ClientInstance_sendChat_fn)(ClientInstance* ci, const char* msg);
    static void (*GameMode_attack_fn)(C_GameMode* gm, C_Entity* entity);
    static void (*LocalPlayer_jump_fn)(void* mcLocalPlayer);
    static bool (*LocalPlayer_isSneaking_fn)(void* mcLocalPlayer);
    static void (*LocalPlayer_setSneaking_fn)(void* mcLocalPlayer, bool sneaking);

    void ensureIntegration() const;
};
