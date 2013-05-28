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

	_running = false;

	return true;
}

void Timer::StartTime()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&_startTime);

	_running = true;
}

float Timer::GetTime()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&_time);

	float timeDif = (float)(_time - _startTime);

	return (timeDif/_ticksPerMs);
}

bool Timer::IsCounting()
{
	return _running;
}