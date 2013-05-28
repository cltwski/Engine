#pragma once

class Keyboard
{
public:
	Keyboard();
	Keyboard(const Keyboard&);
	~Keyboard();


	void Init();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

private:
	bool _keys[256];
};