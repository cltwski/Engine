#include "Timer.h"

Timer::Timer()
{}

Timer::Timer(const Timer& other)
{}

Timer::~Timer()
{}

bool Timer::Init()
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

void Timer::Frame()
{
	INT64 currentTime;
	float timeDif;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	timeDif = (float)(currentTime - _startTime);

	_frameTime = timeDif / _ticksPerMs;

	_startTime = currentTime;
}

float Timer::GetTime()
{
	return _frameTime;
}