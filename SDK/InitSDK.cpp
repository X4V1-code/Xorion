// InitSDK.cpp
#include "InitSDK.h"
#include "VersionTag.h"
#include "Signatures.h"
#include "Utils/Logger.h"
#include "SlimMem.h"

#include "ItemStack.h"
#include "Player.h"
#include "LocalPlayer.h"
#include "PlayerSupplies.h"
#include "SDK/CGameMode.h"
#include "SDK/ClientInstance.h"

static bool g_SdkInitialized = false;

bool InitSDK() {
    logF("[InitSDK] Resolving signatures for Minecraft %s ...", kMinecraftVersion.c_str());

    // ItemStack
    ItemStack::ItemStack_getCount_fn = (int(*)(void*))SlimMem::FindSignature(PATTERN_ItemStack_getCount);
    ItemStack::ItemStack_setCount_fn = (void(*)(void*, int))SlimMem::FindSignature(PATTERN_ItemStack_setCount);
    ItemStack::ItemStack_getName_fn  = (const char*(*)(void*))SlimMem::FindSignature(PATTERN_ItemStack_getName);

    // Player
    Player::Player_getName_fn            = (const char*(*)(void*))SlimMem::FindSignature(PATTERN_Player_getName);
    Player::Player_getItemStackInHand_fn = (void*(*)(void*))SlimMem::FindSignature(PATTERN_Player_getItemStackInHand);
    Player::Player_getPos_fn             = (Vec3(*)(void*))SlimMem::FindSignature(PATTERN_Player_getPos);
    Player::Player_setPos_fn             = (void(*)(void*, const Vec3&))SlimMem::FindSignature(PATTERN_Player_setPos);
    Player::Player_getHealth_fn          = (float(*)(void*))SlimMem::FindSignature(PATTERN_Player_getHealth);
    Player::Player_setHealth_fn          = (void(*)(void*, float))SlimMem::FindSignature(PATTERN_Player_setHealth);

    // LocalPlayer
    LocalPlayer::ClientInstance_sendChat_fn = (void(*)(ClientInstance*, const char*))SlimMem::FindSignature(PATTERN_ClientInstance_sendChat);
    LocalPlayer::GameMode_attack_fn         = (void(*)(C_GameMode*, C_Entity*))SlimMem::FindSignature(PATTERN_GameMode_attack);
    LocalPlayer::LocalPlayer_jump_fn        = (void(*)(void*))SlimMem::FindSignature(PATTERN_LocalPlayer_jump);
    LocalPlayer::LocalPlayer_isSneaking_fn  = (bool(*)(void*))SlimMem::FindSignature(PATTERN_LocalPlayer_isSneaking);
    LocalPlayer::LocalPlayer_setSneaking_fn = (void(*)(void*, bool))SlimMem::FindSignature(PATTERN_LocalPlayer_setSneaking);

    // PlayerSupplies
    PlayerSupplies::PlayerSupplies_getSelectedSlot_fn = (int(*)(void*))SlimMem::FindSignature(PATTERN_PlayerSupplies_getSelectedSlot);
    PlayerSupplies::PlayerSupplies_setSelectedSlot_fn = (void(*)(void*, int))SlimMem::FindSignature(PATTERN_PlayerSupplies_setSelectedSlot);
    PlayerSupplies::PlayerSupplies_getItemStack_fn    = (void*(*)(void*, int))SlimMem::FindSignature(PATTERN_PlayerSupplies_getItemStack);

    // Validation & report
    int resolvedCount = 0;
    resolvedCount += (ItemStack::ItemStack_getCount_fn ? 1 : 0);
    resolvedCount += (ItemStack::ItemStack_getName_fn ? 1 : 0);
    resolvedCount += (Player::Player_getName_fn ? 1 : 0);
    resolvedCount += (Player::Player_getPos_fn ? 1 : 0);
    resolvedCount += (Player::Player_getHealth_fn ? 1 : 0);
    resolvedCount += (LocalPlayer::GameMode_attack_fn ? 1 : 0);
    resolvedCount += (LocalPlayer::ClientInstance_sendChat_fn ? 1 : 0);
    resolvedCount += (PlayerSupplies::PlayerSupplies_getSelectedSlot_fn ? 1 : 0);

    g_SdkInitialized = resolvedCount >= 8; // minimal required core set
    if (g_SdkInitialized)
        logF("[InitSDK] Core signatures resolved for %s (count=%d).", kMinecraftVersion.c_str(), resolvedCount);
    else
        logF("[InitSDK] Missing core signatures for %s (count=%d). SDK will guard calls.", kMinecraftVersion.c_str(), resolvedCount);

    return g_SdkInitialized;
}
