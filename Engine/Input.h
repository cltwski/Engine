#ifndef INPUT_H
#define INPUT_H

class Input
{
public:
	Input();
	Input(const Input&);
	~Input();

	void Init();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

private:
	bool _keys[256];
};

#endif