#ifndef INPUT_H
#define INPUT_H

#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>

class Input
{
public:
	Input();
	Input(const Input&);
	~Input();

	bool Init(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	bool IsEscapePressed();
	void GetMouseLocation(int&, int&);

private:
	IDirectInput8* _directInput;
	IDirectInputDevice8* _keyboard;
	IDirectInputDevice8* _mouse;	
	unsigned char _keyboardState[256];
	DIMOUSESTATE _mouseState;
	int _screenWidth, _screenHeight;
	int _mouseX, _mouseY;

	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();
};

#endif