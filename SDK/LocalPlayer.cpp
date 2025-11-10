#include "LocalPlayer.h"
#include "../../../Utils/Logger.h"
#include "../../../SDK/CGameMode.h"
#include "../../../SDK/ClientInstance.h"
#include "../../../Horion/Module/ModuleManager.h"

void (*LocalPlayer::ClientInstance_sendChat_fn)(ClientInstance*, const char*) = nullptr;
void (*LocalPlayer::GameMode_attack_fn)(C_GameMode*, C_Entity*) = nullptr;
void (*LocalPlayer::LocalPlayer_jump_fn)(void*) = nullptr;
bool (*LocalPlayer::LocalPlayer_isSneaking_fn)(void*) = nullptr;
void (*LocalPlayer::LocalPlayer_setSneaking_fn)(void*, bool) = nullptr;

LocalPlayer::LocalPlayer(void* mcLocalPlayerPtr) : Player(mcLocalPlayerPtr), sneaking(false) {}
LocalPlayer::~LocalPlayer() {}

void LocalPlayer::ensureIntegration() const {
    if (!mcPlayerPtr || !GameMode_attack_fn || !ClientInstance_sendChat_fn) {
        logF("[LocalPlayer] ERROR: Signatures not resolved. Run InitSDK.cpp.");
        throw std::runtime_error("LocalPlayer SDK not initialized");
    }
}

C_GameMode* LocalPlayer::getGameMode() const { return g_Data.getCGameMode(); }
ClientInstance* LocalPlayer::getClientInstance() const { return g_Data.getClientInstance(); }

void LocalPlayer::sendChatMessage(const std::string& msg) {
    ensureIntegration();
    ClientInstance_sendChat_fn(getClientInstance(), msg.c_str());
}

void LocalPlayer::attackEntity(C_Entity* entity) {
    ensureIntegration();
    GameMode_attack_fn(getGameMode(), entity);
}

void LocalPlayer::jump() {
    ensureIntegration();
    LocalPlayer_jump_fn(mcPlayerPtr);
}

bool LocalPlayer::isSneaking() const {
    ensureIntegration();
    return LocalPlayer_isSneaking_fn(mcPlayerPtr);
}

void LocalPlayer::setSneaking(bool s) {
    ensureIntegration();
    LocalPlayer_setSneaking_fn(mcPlayerPtr, s);
}
