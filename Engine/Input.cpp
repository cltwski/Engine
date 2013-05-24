#include "Input.h"

Input::Input()
{}

Input::Input(const Input& other)
{}

Input::~Input()
{}

void Input::Init()
{
	int i;

	//Init all keys to being release and not being pressed
	for(i=0; i<256; ++i)
	{
		_keys[i] = false;
	}

	return;
}

void Input::KeyDown(unsigned int input)
{
	_keys[input] = true;
}

void Input::KeyUp(unsigned int input)
{
	_keys[input] = false;
}

bool Input::IsKeyDown(unsigned int key)
{
	return _keys[key];
}