#pragma once

class Mouse
{
public:
	Mouse();
	Mouse(const Mouse&);
	~Mouse();

	void Init();

	void SetX(int);
	void SetY(int);
	void SetLButton(bool);
	void SetMButton(bool);
	void SetRButton(bool);

	int GetX();
	int GetY();
	bool LButtonDown();
	bool MButtonDown();
	bool RButtonDown();

private:
	int _x;
	int _y;

	bool _lbutton;
	bool _mbutton;
	bool _rbutton;
};