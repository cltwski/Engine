#include "InputClass.h"

InputClass::InputClass()
{}

InputClass::InputClass(const InputClass& other)
{}

InputClass::~InputClass()
{}

void InputClass::Init()
{
	int i;

	//Init all keys to being release and not being pressed
	for(i=0; i<256; ++i)
	{
		_mKeys[i] = false;
	}

	return;
}

void InputClass::KeyDown(unsigned int input)
{
	_mKeys[input] = true;
}

void InputClass::KeyUp(unsigned int input)
{
	_mKeys[input] = false;
}

bool InputClass::IsKeyDown(unsigned int key)
{
	return _mKeys[key];
}