#include "HideCommand.h"
#include "../../../Utils/TextFormat.h"

HideCommand::HideCommand() : IMCCommand("hide", "hide the mod", "") {
}

HideCommand::~HideCommand() {
}

bool HideCommand::execute(std::vector<std::string>* args) {
	g_Data.setHide(!g_Data.getHide());
	if (g_Data.getHide()) {
		clientMessageF("[%sHorion%s] %sHidden.", GOLD, WHITE, GREEN);
	} else {
		clientMessageF("[%sHorion%s] %sMod is now visible.", GOLD, WHITE, GREEN);
	}
	return true;
}
