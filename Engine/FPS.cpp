#include "FPS.h"

FPS::FPS()
{}

FPS::FPS(const FPS& other)
{}

FPS::~FPS()
{}

void FPS::Init()
{
	_fps = 0;
	_count = 0;
	_startTime = timeGetTime();
}

void FPS::Frame()
{
	_count++;

	if (timeGetTime() >= (_startTime + 1000))
	{
		_fps = _count;
		_count = 0;

		_startTime = timeGetTime();
	}
}

int FPS::GetFps()
{
	return _fps;
}