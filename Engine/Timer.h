#pragma once

#include <Windows.h>

class Timer
{
public:
	Timer();
	Timer(const Timer&);
	~Timer();

	bool Init();
	void StartTime();
	float GetTime();
	bool IsCounting();

private:
	INT64 _frequency;
	float _ticksPerMs;
	INT64 _startTime;
	float _time;
	bool _running;
};