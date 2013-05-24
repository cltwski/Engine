#include "System.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstace, PSTR pScmdline, int iCmdShow)
{
	System* system;
	bool result;

	//Create the system object
	system = new System();
	if (!system)
		return 0;

	//Init and run the system object
	result = system->Init();
	if (result)
	{
		system->Run();
	}

	//Shutdown and release the system object
	system->Shutdown();
	delete system;
	system = NULL;

	return 0;
}