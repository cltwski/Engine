#ifndef SYSTEM_H
#define SYSTEM_H

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include "Input.h"
#include "Graphics.h"
#include "EngineSettings.h"

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

	Input* _input;
	Graphics* _graphics;

	bool Frame();
	void InitWindows(int&, int&);
	void ShutdownWindows();
};

static LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

static System* ApplicationHandle = NULL;

#endif