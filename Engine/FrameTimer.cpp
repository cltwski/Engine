#include "FrameTimer.h"

FrameTimer::FrameTimer()
{}

FrameTimer::FrameTimer(const FrameTimer& other)
{}

FrameTimer::~FrameTimer()
{}

bool FrameTimer::Init()
{
	//Check to see if this system supports high performance timers
	QueryPerformanceFrequency((LARGE_INTEGER*)&_frequency);
	if (_frequency == 0)
		return false;

	//Find out how many time the frequency counter ticks every ms
	_ticksPerMs = (float) (_frequency / 1000);

	QueryPerformanceCounter((LARGE_INTEGER*)&_startTime);

	return true;
}

void FrameTimer::Frame()
{
	INT64 currentTime;
	float timeDif;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	timeDif = (float)(currentTime - _startTime);

	_frameTime = timeDif / _ticksPerMs;

	_startTime = currentTime;
}

float FrameTimer::GetTime()
{
	return _frameTime;
}