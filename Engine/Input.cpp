#include "Input.h"

Input::Input()
{
	_directInput = NULL;
	_keyboard = NULL;
	_mouse = NULL;
}

Input::Input(const Input& other)
{}

Input::~Input()
{}

bool Input::Init(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result;

	_screenWidth = screenWidth;
	_screenHeight = screenHeight;

	_mouseX = 0;
	_mouseY = 0;

	//init the main direct input interface
	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&_directInput, NULL);
	if (FAILED(result))
		return false;

	//Init the interface for the keyboard
	result = _directInput->CreateDevice(GUID_SysKeyboard, &_keyboard, NULL);
	if (FAILED(result))
		return false;

	//Set the data format. in this case it is a keyboard we can use the predefined data format
	result = _keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
		return false;

	//Set the cooperative level of the keyboard not share with other program
	result = _keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
		return false;

	//Now acquire the keyboard
	result = _keyboard->Acquire();
	if (FAILED(result))
		return false;

	//Next setup the mouse
	result = _directInput->CreateDevice(GUID_SysMouse, &_mouse, NULL);
	if (FAILED(result))
		return false;

	//Set the data format to the pre defined format
	result = _mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
		return false;

	//Set the coop level to share with other programs
	result = _mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
		return false;

	//Acquire the mouse
	result = _mouse->Acquire();
	if (FAILED(result))
		return false;

	return true;
}

void Input::Shutdown()
{
	//Release the mouse
	if (_mouse)
	{
		_mouse->Unacquire();
		_mouse->Release();
		_mouse = NULL;
	}

	//Release the keyboard
	if (_keyboard)
	{
		_keyboard->Unacquire();
		_keyboard->Release();
		_keyboard = NULL;
	}

	//Releaswe the main interface to direct input
	if (_directInput)
	{
		_directInput->Release();
		_directInput = NULL;
	}
}

bool Input::Frame()
{
	bool result;

	//Read the current state of the keyboard
	result = ReadKeyboard();
	if (!result)
		return false;

	//Read the state of the mouse
	result = ReadMouse();
	if (!result)
		return false;

	//Process the changes in the mouse and keyboard
	ProcessInput();

	return true;
}

bool Input::ReadKeyboard()
{
	HRESULT result;

	//Read the keyboard device
	result = _keyboard->GetDeviceState(sizeof(_keyboardState), (LPVOID)&_keyboardState);
	if (FAILED(result))
	{
		//if the keyboard lost focus or was not acquired then try to get control back
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool Input::ReadMouse()
{
	HRESULT result;

	//Read the mouse device
	result = _mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&_mouseState);
	if (FAILED(result))
	{
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
			_mouse->Acquire();
		else
			return false;
	}

	return true;
}

void Input::ProcessInput()
{
	//Update the location of the mouse curosr based on the change of the mouse location during the frame
	_mouseX += _mouseState.lX;
	_mouseY += _mouseState.lY;

	//Ensure that the mouse location doesn't exceed the screen width or height
	if (_mouseX < 0)
		_mouseX = 0;
	if (_mouseY < 0)
		_mouseY = 0;

	if (_mouseX > _screenWidth)
		_mouseX = _screenWidth;
	if (_mouseY > _screenHeight)
		_mouseY = _screenHeight;
}

bool Input::IsEscapePressed()
{
	//Do a bitwise and on thekeyboard state to check if the escape key is currently being pressed
	if (_keyboardState[DIK_ESCAPE] & 0x80)
		return true;

	return false;
}

void Input::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = _mouseX;
	mouseY = _mouseY;
}
