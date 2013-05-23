#ifndef SYSTEMCLASS_H
#define SYSTEMCLASS_H

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include "InputClass.h"
#include "GraphicsClass.h"
#include "EngineSettings.h"

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Init();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	LPCWSTR m_ApplicationName;
	HINSTANCE m_HInstance;
	HWND m_HWnd;

	InputClass* m_Input;
	GraphicsClass* m_Graphics;

	bool Frame();
	void InitWindows(int&, int&);
	void ShutdownWindows();
};

static LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

static SystemClass* ApplicationHandle = NULL;

#endif