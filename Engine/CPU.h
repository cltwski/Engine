#ifndef CPU_H
#define CPU_H

#pragma comment(lib, "pdh.lib")

#include <Pdh.h>

class CPU
{
public:
	CPU();
	CPU(const CPU&);
	~CPU();

	void Init();
	void Shutdown();
	void Frame();
	int GetCPUPercent();

private:
	bool _canReadCpu;
	HQUERY _queryHandle;
	HCOUNTER _counterHandle;
	unsigned long _lastSampleTime;
	long _cpuUsage;
};

#endif