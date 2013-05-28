#ifndef SYSTEM_H
#define SYSTEM_H

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "EngineSettings.h"
#include "FPS.h"
#include "CPU.h"
#include "FrameTimer.h"
#include "EnumGameState.h"
#include "Timer.h"

class System
{
public:
	System();
	System(const System&);
	~System();

	bool Init();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	LPCWSTR _applicationName;
	HINSTANCE _hInstance;
	HWND _hWnd;

	Keyboard* _keyboard;
	Mouse* _mouse;
	Graphics* _graphics;
	FPS* _fps;
	CPU* _cpu;
	FrameTimer* _frameTimer;
	Timer* _timer;
	GameState _state;

	bool Frame();
	void InitWindows(int&, int&);
	void ShutdownWindows();
};

static LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

static System* ApplicationHandle = NULL;

#endif