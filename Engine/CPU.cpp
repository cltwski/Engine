#include "CPU.h"

CPU::CPU()
{}

CPU::CPU(const CPU& other)
{}

CPU::~CPU()
{}

void CPU::Init()
{
	PDH_STATUS status;

	//Init the flag indicating whether this object can the system cpu usage or not
	_canReadCpu = true;

	//Create a query object to poll cpu usage
	status = PdhOpenQuery(NULL, 0, &_queryHandle);
	if (status != ERROR_SUCCESS)
		_canReadCpu = false;

	//Set query object to poll all cpu in the system
	status = PdhAddCounter(_queryHandle, TEXT("\\Processor(_Total)\\% processor time"), 0, &_counterHandle);
	if (status != ERROR_SUCCESS)
		_canReadCpu = false;

	_lastSampleTime = GetTickCount();

	_cpuUsage = 0;
}

void CPU::Shutdown()
{
	if (_canReadCpu)
		PdhCloseQuery(_queryHandle);
}

void CPU::Frame()
{
	PDH_FMT_COUNTERVALUE value;

	if (_canReadCpu)
	{
		if ((_lastSampleTime + 1000) < GetTickCount())
		{
			_lastSampleTime = GetTickCount();
			PdhCollectQueryData(_queryHandle);
			PdhGetFormattedCounterValue(_counterHandle, PDH_FMT_LONG, NULL, &value);
			_cpuUsage = value.longValue;
		}
	}
}

int CPU::GetCPUPercent()
{
	int usage;

	if (_canReadCpu)
		usage = (int)_cpuUsage;
	else
		usage = 0;
	
	return usage;
}


