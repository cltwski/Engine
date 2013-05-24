#include "EngineSettings.h"

EngineSettings::EngineSettings()
{
	_fullscreen = false;
	_vsync = true;
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

	//Get _fullscreen defaults to false
	result = GetPrivateProfileInt(L"Graphics", L"Fullscreen", 0, file);
	_fullscreen = (result==1) ? true : false;

	//Get vsync defaults to true
	result = GetPrivateProfileInt(L"Graphics", L"Vsync", 1, file);
	_vsync = (result==1) ? true : false;
}

bool EngineSettings::GetFullScreen()
{
	return _fullscreen;
}

bool EngineSettings::GetVsync()
{
	return _vsync;
}
