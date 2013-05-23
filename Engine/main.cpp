#include "SystemClass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstace, PSTR pScmdline, int iCmdShow)
{
	SystemClass* System;
	bool result;

	//Create the system object
	System = new SystemClass();
	if (!System)
		return 0;

	//Init and run the system object
	result = System->Init();
	if (result)
	{
		System->Run();
	}

	//Shutdown and release the system object
	System->Shutdown();
	delete System;
	System = NULL;

	return 0;
}