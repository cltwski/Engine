#include "System.h"

#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <iostream>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstace, PSTR pScmdline, int iCmdShow)
{
	//For making a console window
	AllocConsole();

	HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle((long)handle_out, _O_TEXT);
	FILE* hf_out = _fdopen(hCrt, "w");
	setvbuf(hf_out, NULL, _IONBF, 1);
	*stdout = *hf_out;

	HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
	hCrt = _open_osfhandle((long) handle_in, _O_TEXT);
	FILE* hf_in = _fdopen(hCrt, "r");
	setvbuf(hf_in, NULL, _IONBF, 128);
	*stdin = *hf_in;
	//End of console section

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