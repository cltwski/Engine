#include "EngineSettings.h"

EngineSettings::EngineSettings()
{
	fullscreen = false;
	vsync = true;
}

EngineSettings& EngineSettings::GetInstance()
{
	static EngineSettings instance;
	return instance;
}

void EngineSettings::LoadFromFile()
{
	UINT result = 0;

	LPCWSTR file = L"G:\\Users\\Christopher\\Google Drive\\Game Project\\Engine\\Engine\\Settings.ini";

	//Get fullscreen defaults to false
	result = GetPrivateProfileInt(L"Graphics", L"Fullscreen", 0, file);
	fullscreen = (result==1) ? true : false;

	//Get vsync defaults to true
	result = GetPrivateProfileInt(L"Graphics", L"Vsync", 1, file);
	vsync = (result==1) ? true : false;
}

bool EngineSettings::GetFullScreen()
{
	return fullscreen;
}

bool EngineSettings::GetVsync()
{
	return vsync;
}
