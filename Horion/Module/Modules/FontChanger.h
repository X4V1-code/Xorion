#pragma once
#include "Module.h"

class FontChanger : public IModule {
public:
	// TODO: SettingEnum removed in 1.21.123
	//SettingEnum Fonts = SettingEnum(this);
	FontChanger();
	~FontChanger();

	// Inherited via IModule
	virtual const char* getModuleName() override;
};