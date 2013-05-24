#ifndef TIMER_H
#define TIMER_H

#include <Windows.h>

class Timer
{
public:
	Timer();
	Timer(const Timer&);
	~Timer();

	bool Init();
	void Frame();

	float GetTime();
private:
	INT64 _frequency;
	float _ticksPerMs;
	INT64 _startTime;
	float _frameTime;
};

#endif