#ifndef TIMER_H
#define TIMER_H

#include <Windows.h>

class FrameTimer
{
public:
	FrameTimer();
	FrameTimer(const FrameTimer&);
	~FrameTimer();

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