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

private:
	static EngineSettings* instance;

	EngineSettings();
	EngineSettings(EngineSettings const&);
	void operator=(EngineSettings const&);

private:
	bool fullscreen;
	bool vsync;
};

#endif

