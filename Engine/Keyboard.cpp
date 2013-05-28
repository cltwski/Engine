#include "Keyboard.h"

Keyboard::Keyboard()
{}

Keyboard::Keyboard(const Keyboard& other)
{}

Keyboard::~Keyboard()
{}

void Keyboard::Init()
{
	//Init all the keyboard ot not being pressed
	for (int i=0; i < 256; ++i)
		_keys[i] = false;
}

void Keyboard::KeyDown(unsigned int input)
{
	//Update state of key pressed
	_keys[input] = true;
}

void Keyboard::KeyUp(unsigned int input)
{
	//Update state of key pressed
	_keys[input] = true;
}

bool Keyboard::IsKeyDown(unsigned int key)
{
	return _keys[key];
}