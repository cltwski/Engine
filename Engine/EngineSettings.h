#ifndef ENGINESETTINGS_H
#define ENGINESETTINGS_H

#include <Windows.h>

class EngineSettings
{
public:
	static EngineSettings& GetInstance();

	void LoadFromFile();

	bool GetFullScreen();
	bool GetVsync();
	int GetWindowX();
	int GetWindowY();
	int GetCurrentWindowX();
	int GetCurrentWindowY();
	void SetCurrentWindowX(int);
	void SetCurrentWindowY(int);

private:
	EngineSettings();
	EngineSettings(EngineSettings const&);
	void operator=(EngineSettings const&);

private:
	bool _fullscreen;
	bool _vsync;
	int _windowX;
	int _windowY;
	int _currentWindowX;
	int _currentWindowY;
};

#endif

