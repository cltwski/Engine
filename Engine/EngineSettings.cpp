#include "EngineSettings.h"

EngineSettings::EngineSettings()
{
	_fullscreen = false;
	_vsync = true;
	_windowX = 640;
	_windowY = 480;
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

	//Get window X
	_windowX = GetPrivateProfileInt(L"Graphics", L"WindowX", 640, file);

	//Get window Y
	_windowY = GetPrivateProfileInt(L"Graphics", L"WindowY", 480, file);
}

bool EngineSettings::GetFullScreen()
{
	return _fullscreen;
}

bool EngineSettings::GetVsync()
{
	return _vsync;
}

int EngineSettings::GetWindowX()
{
	return _windowX;
}

int EngineSettings::GetWindowY()
{
	return _windowY;
}
