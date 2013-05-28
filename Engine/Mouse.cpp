#include "Mouse.h"

Mouse::Mouse()
{}

Mouse::Mouse(const Mouse&)
{}

Mouse::~Mouse()
{}

void Mouse::Init()
{
	_x = -1;
	_y = -1;

	_lbutton = false;
	_mbutton = false;
	_rbutton = false;
}

void Mouse::SetX(int x)
{
	_x = x;
}

void Mouse::SetY(int y)
{
	_y = y;
}

void Mouse::SetLButton(bool l)
{
	_lbutton = l;
}

void Mouse::SetMButton(bool m)
{
	_mbutton = m;
}

void Mouse::SetRButton(bool r)
{
	_rbutton = r;
}

int Mouse::GetX()
{
	return _x;
}

int Mouse::GetY()
{
	return _y;
}

bool Mouse::LButtonDown()
{
	return _lbutton;
}

bool Mouse::MButtonDown()
{
	return _mbutton;
}

bool Mouse::RButtonDown()
{
	return _rbutton;
}
