#include "GameModeCommand.h"

GameModeCommand::GameModeCommand() : IMCCommand("gamemode", "BEND REALITY TO YOUR WILL - Transcend mortal limitations and become UNSTOPPABLE", "<number>") {
	registerAlias("gm");
}

GameModeCommand::~GameModeCommand() {
}

bool GameModeCommand::execute(std::vector<std::string>* args) {
	assertTrue(Game.getLocalPlayer() != nullptr);
	assertTrue(args->size() > 1);

	int gamemode = assertInt(args->at(1));

	if (gamemode >= 0 && gamemode <= 6) {
		Game.getLocalPlayer()->setPlayerGameType(static_cast<GameType>(gamemode));

		// MAXIMUM POWER - Aggressive and dominating messaging
		switch (gamemode) {
		case 0:  // Survival
			clientMessageF("[%sXorion%s] %s%s>>> SURVIVAL DOMINATION ENGAGED <<<", GOLD, WHITE, RED, BOLD);
			clientMessageF("[%sXorion%s] %s%sTHE WEAK FEAR YOU. THE STRONG RESPECT YOU.", GOLD, WHITE, DARK_RED, BOLD);
			clientMessageF("[%sXorion%s] %sYou are the %sAPEX PREDATOR%s - %sNOTHING%s can stop your wrath!",
						   GOLD, WHITE, YELLOW, RED, YELLOW, DARK_RED, WHITE);
			clientMessageF("[%sXorion%s] %s%sCRUSH. CONQUER. DOMINATE.", GOLD, WHITE, RED, BOLD);
			break;
		case 1:  // Creative
			clientMessageF("[%sXorion%s] %s%s<<< ABSOLUTE GOD MODE ACTIVATED >>>", GOLD, WHITE, LIGHT_PURPLE, BOLD);
			clientMessageF("[%sXorion%s] %s%sYOU ARE NOW OMNIPOTENT. INFINITE. ETERNAL.", GOLD, WHITE, AQUA, BOLD);
			clientMessageF("[%sXorion%s] %sReality is your %sPLAYTHING%s - Create. Destroy. %sANNIHILATE.",
						   GOLD, WHITE, WHITE, LIGHT_PURPLE, WHITE, DARK_RED);
			clientMessageF("[%sXorion%s] %s%sNO LIMITS. NO MERCY. ONLY POWER.", GOLD, WHITE, RED, BOLD);
			clientMessageF("[%sXorion%s] %sThe universe %sTREMBLES%s before your might!", GOLD, WHITE, GRAY, LIGHT_PURPLE, WHITE);
			break;
		case 2:  // Adventure
			clientMessageF("[%sXorion%s] %s%s>>> LEGENDARY CONQUEROR MODE <<<", GOLD, WHITE, GOLD, BOLD);
			clientMessageF("[%sXorion%s] %s%sYOU ARE THE CHOSEN ONE - DESTINED FOR GLORY!", GOLD, WHITE, YELLOW, BOLD);
			clientMessageF("[%sXorion%s] %sEnemies will %sFALL%s before you. Kingdoms will %sBURN%s at your command!",
						   GOLD, WHITE, GRAY, RED, GRAY, DARK_RED, WHITE);
			clientMessageF("[%sXorion%s] %s%sYOUR LEGEND BEGINS NOW.", GOLD, WHITE, GOLD, BOLD);
			break;
		case 3:  // Spectator
			clientMessageF("[%sXorion%s] %s%s<<< VOID WALKER MODE ACTIVATED >>>", GOLD, WHITE, DARK_PURPLE, BOLD);
			clientMessageF("[%sXorion%s] %s%sYOU HAVE TRANSCENDED PHYSICAL FORM!", GOLD, WHITE, LIGHT_PURPLE, BOLD);
			clientMessageF("[%sXorion%s] %sPhase through %sREALITY%s like a %sDARK ENTITY%s from the void!",
						   GOLD, WHITE, GRAY, DARK_AQUA, GRAY, DARK_PURPLE, WHITE);
			clientMessageF("[%sXorion%s] %s%sINVISIBLE. UNTOUCHABLE. OMNIPRESENT.", GOLD, WHITE, DARK_PURPLE, BOLD);
			clientMessageF("[%sXorion%s] %sWatch. Learn. %sSTRIKE%s when they least expect!", GOLD, WHITE, GRAY, RED, WHITE);
			break;
		default:
			clientMessageF("[%sXorion%s] %s%s>>> FORBIDDEN DIMENSION BREACH <<<", GOLD, WHITE, BLUE, BOLD);
			clientMessageF("[%sXorion%s] %s%sGAMEMODE %d - REALITY MATRIX OVERWRITTEN!", GOLD, WHITE, AQUA, BOLD, gamemode);
			clientMessageF("[%sXorion%s] %sYou have %sBROKEN%s the laws of existence!", GOLD, WHITE, GRAY, RED, WHITE);
			clientMessageF("[%sXorion%s] %s%sUNLIMITED POWER FLOWS THROUGH YOU!", GOLD, WHITE, LIGHT_PURPLE, BOLD);
			break;
		}
	} else {
		clientMessageF("[%sXorion%s] %s%s!!! CRITICAL ERROR: REALITY REJECTION !!!", GOLD, WHITE, RED, BOLD);
		clientMessageF("[%sXorion%s] %s%sINVALID GAMEMODE - THE VOID REJECTS YOUR COMMAND!", GOLD, WHITE, DARK_RED, BOLD);
		clientMessageF("[%sXorion%s] %sEven %sGODS%s must follow the rules: Use %s0-6%s to dominate!",
					   GOLD, WHITE, GRAY, LIGHT_PURPLE, GRAY, YELLOW, WHITE);
		clientMessageF("[%sXorion%s] %s%sTRY AGAIN - YOUR DESTINY AWAITS!", GOLD, WHITE, GOLD, BOLD);
	}

	return true;
}
