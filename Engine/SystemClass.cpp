#include "SystemClass.h"

SystemClass::SystemClass()
{
	_mInput = NULL;
	_mGraphics = NULL;
}

SystemClass::SystemClass(const SystemClass& other)
{}

SystemClass::~SystemClass()
{}

bool SystemClass::Init()
{
	int screenWidth = 0;
	int screenHeight = 0;
	bool result;

	//Init the windows api
	InitWindows(screenWidth, screenHeight);

	//Create input object
	_mInput = new InputClass();
	if (!_mInput)
		return false;

	//Init input object
	_mInput->Init();

	//Create graphics object
	_mGraphics = new GraphicsClass();
	if (!_mGraphics)
		return false;

	//Init graphics object
	result = _mGraphics->Init(screenWidth, screenHeight, _mHWnd);
	if (!result)
		return false;

	return true;
}

void SystemClass::Shutdown()
{
	//Release graphics object
	if (_mGraphics)
	{
		_mGraphics->Shutdown();
		delete _mGraphics;
		_mGraphics = NULL;
	}

	//Release the input object
	if (_mInput)
	{
		delete _mInput;
		_mInput = NULL;
	}

	//Shutdown the window
	ShutdownWindows();
}

void SystemClass::Run()
{
	MSG msg;
	bool done, result;

	//Init the message structure
	ZeroMemory(&msg, sizeof(MSG));

	//Loop until there is a quit message from the window or the user
	done = false;
	while (!done)
	{
		//Handle windows messages
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//If quit message received then quit
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			//Otherwise continue
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
	}
}

bool SystemClass::Frame()
{
	bool result;

	//Check if the user pressed escape key (to quit)
	if (_mInput->IsKeyDown(VK_ESCAPE))
		return false;

	//Do game processing here

	//Do frame processing for the graphics object
	result = _mGraphics->Frame();
	if (!result)
		return false;

	return true;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hWnd, UINT umsg, WPARAM wParam, LPARAM lParam)
{
	switch (umsg)
	{
		//Check if a key has been pressed on the keyboard
	case WM_KEYDOWN:
		{
			_mInput->KeyDown((unsigned int)wParam);
			return 0;
		}

		//Check if a key has been released on the keyboard
	case WM_KEYUP:
		{
			_mInput->KeyUp((unsigned int)wParam);
			return 0;
		}

		//Any other messages send to the default message handler as our application wont use them
	default:
		{
			return DefWindowProc(hWnd, umsg, wParam, lParam);
		}
	}
}


void SystemClass::InitWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	//Get an external pointer to this object;
	ApplicationHandle = this;

	//Get the instance of the application
	_mHInstance = GetModuleHandle(NULL);

	//Give the application a name
	_mApplicationName = L"Engine";

	//Setup the windows class with default settings;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = _mHInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = _mApplicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	//Register the window class
	RegisterClassEx(&wc);

	//Determine the resolution of the desktop screen
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	//Setup the screen settings depending on whether it is running in full screen or in windows mode
	if (FULL_SCREEN)
	{
		//If full screen set the screen to maximum size of the desktop and 32 bit
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		//Change the displat settings to full screen
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		//Set the position of the window to the top left corner
		posX = 0;
		posY = 0;
	}
	else
	{
		//If windowed then set it to 800x600 resolution
		screenWidth = 800;
		screenHeight = 600;

		//Place the window in the middle of the screen
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth)/2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight)/2;
	}

	//Create the window with the screen settings and get the handle to it
	_mHWnd = CreateWindowEx(WS_EX_APPWINDOW, _mApplicationName, _mApplicationName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, posX, posY, screenWidth, screenHeight, NULL, NULL, _mHInstance, NULL);

	//Bring the window up on the screen and set it as the main focus
	ShowWindow(_mHWnd, SW_SHOW);
	SetForegroundWindow(_mHWnd);
	SetFocus(_mHWnd);

	//Hide the mouse cursor
	ShowCursor(false);
}

void SystemClass::ShutdownWindows()
{
	//Show the mouse cursor
	ShowCursor(true);

	//Fix the display settings if leaving full screen mode
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL,0);
	}

	//Remove the window
	DestroyWindow(_mHWnd);
	_mHWnd = NULL;

	//Remove the application instance
	UnregisterClass(_mApplicationName, _mHInstance);
	_mHInstance = NULL;

	//Release the pointer to this class
	ApplicationHandle = NULL;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT umsg, WPARAM wParam, LPARAM lParam)
{
	switch (umsg)
	{
		//Check if the window is being destroyed
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		//Check if the window is being cloased
	case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		//All other messages pass to the message handler in the sustem class
	default:
		{
			return ApplicationHandle->MessageHandler(hWnd, umsg, wParam, lParam);
		}
	}
}

