#pragma once

#include "Module.h"
#include "../ModuleManager.h"

class FullAuto : public IModule {
private:
	// Section 1: Perfection Mode
	bool perfectionMode = false;
	
	// Section 2: Hacks
	int hacksMode = 0;  // 0 = Off, 1 = Low Risk, 2 = On
	
	// Section 3: Mode
	int mode = 0;  // 0 = Kill, 1 = Run
	
	// Internal state tracking
	int autoclickerBurstTicks = 0;
	int autoclickerBurstDuration = 0;  // in ticks (2.5 seconds = 50 ticks)
	bool isInBurst = false;
	
	// Helper methods
	void handleKillMode(Entity* player, C_GameMode* gm);
	void handleRunMode(Entity* player, C_GameMode* gm);
	void enableLowRiskHacks();
	void enableFullHacks();
	void disableAllHacks();
	bool shouldPlaceFail();  // Returns true if scaffold should fail (1/18 chance)
	bool shouldAimbotMiss();  // Returns true if aimbot should miss slightly (1/6 chance)
	bool shouldFuckerIgnore();  // Returns true if fucker should ignore (30% chance)
	Entity* findNearestPlayer(Entity* localPlayer);
	void findAndRunFromPlayers(Entity* localPlayer, C_GameMode* gm);
	bool isOpponentSidestepping(Entity* target);
	bool shouldAirJumpForSafety(Entity* player);

public:
	FullAuto();
	~FullAuto();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onPlayerTick(Player* player) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
};
