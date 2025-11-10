#include "Player.h"
#include "../../../Utils/Logger.h"

const char* (*Player::Player_getName_fn)(void*) = nullptr;
void*       (*Player::Player_getItemStackInHand_fn)(void*) = nullptr;
Vec3        (*Player::Player_getPos_fn)(void*) = nullptr;
void        (*Player::Player_setPos_fn)(void*, const Vec3&) = nullptr;
float       (*Player::Player_getHealth_fn)(void*) = nullptr;
void        (*Player::Player_setHealth_fn)(void*, float) = nullptr;

Player::Player(void* mcPlayerPtr) : C_Entity(mcPlayerPtr), mcPlayerPtr(mcPlayerPtr) {}
Player::~Player() {}

void Player::ensureIntegration() const {
    if (!mcPlayerPtr || !Player_getName_fn || !Player_getPos_fn || !Player_getHealth_fn) {
        logF("[Player] ERROR: Signatures not resolved. Run InitSDK.cpp.");
        throw std::runtime_error("Player SDK not initialized");
    }
}

std::string Player::getName() const {
    ensureIntegration();
    return std::string(Player_getName_fn(mcPlayerPtr));
}

ItemStack Player::getHeldItem() const {
    ensureIntegration();
    void* itemPtr = Player_getItemStackInHand_fn(mcPlayerPtr);
    return ItemStack(itemPtr);
}

Vec3 Player::getPos() const {
    ensureIntegration();
    return Player_getPos_fn(mcPlayerPtr);
}

void Player::setPos(const Vec3& pos) {
    ensureIntegration();
    Player_setPos_fn(mcPlayerPtr, pos);
}

float Player::getHealth() const {
    ensureIntegration();
    return Player_getHealth_fn(mcPlayerPtr);
}

void Player::setHealth(float health) {
    ensureIntegration();
    Player_setHealth_fn(mcPlayerPtr, health);
}
