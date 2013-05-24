#ifndef FPS_H
#define FPS_H

#pragma comment(lib, "winmm.lib")

#include <Windows.h>
#include <mmstream.h>

class FPS
{
public:
	FPS();
	FPS(const FPS&);
	~FPS();

	void Init();
	void Frame();
	int GetFps();

private:
	int _fps, _count;
	unsigned long _startTime;
};

#endif