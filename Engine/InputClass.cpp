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
		m_Keys[i] = false;
	}

	return;
}

void InputClass::KeyDown(unsigned int input)
{
	m_Keys[input] = true;
}

void InputClass::KeyUp(unsigned int input)
{
	m_Keys[input] = false;
}

bool InputClass::IsKeyDown(unsigned int key)
{
	return m_Keys[key];
}