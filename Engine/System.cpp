#include "System.h"

System::System()
{
	_keyboard = NULL;
	_mouse = NULL;
	_graphics = NULL;
	_fps = NULL;
	_cpu = NULL;
	_timer = NULL;
}

System::System(const System& other)
{}

System::~System()
{}

bool System::Init()
{
	int screenWidth = 0;
	int screenHeight = 0;
	bool result;

	//Init Settings
	EngineSettings::GetInstance().LoadFromFile();

	//Init the windows api
	InitWindows(screenWidth, screenHeight);

	//Create the keyboard and init
	_keyboard = new Keyboard();
	_keyboard->Init();

	//Create the mouse and init
	_mouse = new Mouse();
	_mouse->Init();

	//Create graphics object
	_graphics = new Graphics();
	if (!_graphics)
		return false;

	//Init graphics object
	result = _graphics->Init(screenWidth, screenHeight, _hWnd);
	if (!result)
		return false;

	//Create and init FPS object
	_fps = new FPS();
	if (!_fps)
		return false;
	_fps->Init();

	//Create and init the CPU objet
	_cpu = new CPU();
	if (!_cpu)
		return false;
	_cpu->Init();

	//Create and init the Timer object
	_timer = new Timer();
	if (!_timer)
		return false;
	result = _timer->Init();
	if (!result)
	{
		MessageBox(_hWnd, L"Could not init the timer object", L"Error", MB_OK);
		return false;
	}

	return true;
}

void System::Shutdown()
{
	//Release the timer object
	if (_timer)
	{
		delete _timer;
		_timer = NULL;
	}

	//Release the CPU object
	if (_cpu)
	{
		_cpu->Shutdown();
		delete _cpu;
		_cpu = NULL;
	}

	//Release the FPS object
	if (_fps)
	{
		delete _fps;
		_fps = NULL;
	}

	//Release graphics object
	if (_graphics)
	{
		_graphics->Shutdown();
		delete _graphics;
		_graphics = NULL;
	}

	//Release the input objects
	if (_keyboard)
	{
		delete _keyboard;
		_keyboard = NULL;
	}

	if (_mouse)
	{
		delete _mouse;
		_mouse = NULL;
	}

	//Shutdown the window
	ShutdownWindows();
}

void System::Run()
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
				MessageBox(_hWnd, L"Frame processing failed", L"Error", MB_OK);
				done = true;
			}
		}

		//Check to see if the user pressed escape and wants to quit
		if (_keyboard->IsKeyDown(VK_ESCAPE))
			done = true;
	}
}

bool System::Frame()
{
	bool result;
	int mouseX, mouseY;

	//Update the system stats
	_timer->Frame();
	_fps->Frame();
	_cpu->Frame();

	//Update from inputs
	mouseX = _mouse->GetX();
	mouseY = _mouse->GetY();

	//Do game processing here

	//Do frame processing for the graphics object
	result = _graphics->Frame(mouseX, mouseY, _fps->GetFps(), _cpu->GetCPUPercent(), _timer->GetTime());
	if (!result)
		return false;

	result = _graphics->Render();
	if (!result)
		return false;

	return true;
}

LRESULT CALLBACK System::MessageHandler(HWND hWnd, UINT umsg, WPARAM wParam, LPARAM lParam)
{
	switch (umsg)
	{
		//Handle keyboard cases
	case WM_KEYDOWN:
		{
			_keyboard->KeyDown((int)wParam);

			return 0;
		}
	case WM_KEYUP:
		{
			_keyboard->KeyUp((int)wParam);

			return 0;
		}
		//Handle mouse cases
	case WM_MOUSEMOVE:
		{
			//Update position
			_mouse->SetX((int)LOWORD(lParam));
			_mouse->SetY((int)HIWORD(lParam));

			//get buttons state
			int buttons = (int)wParam;

			//Check buttons state
			if (buttons & MK_LBUTTON)
				_mouse->SetLButton(true);
			else
				_mouse->SetLButton(false);

			if (buttons & MK_MBUTTON)
				_mouse->SetMButton(true);
			else
				_mouse->SetMButton(false);

			if (buttons & MK_RBUTTON)
				_mouse->SetRButton(true);
			else
				_mouse->SetRButton(false);

			if (buttons & MK_CONTROL)
				_keyboard->KeyDown(VK_CONTROL);
			else
				_keyboard->KeyUp(VK_CONTROL);

			if (buttons & MK_SHIFT)
				_keyboard->KeyDown(VK_SHIFT);
			else
				_keyboard->KeyUp(VK_SHIFT);

			return 0;
		}
		//Handle mouse clicks
	case WM_MBUTTONDOWN:
		{
			//Update position
			_mouse->SetX((int)LOWORD(lParam));
			_mouse->SetY((int)HIWORD(lParam));

			_mouse->SetMButton(true);

			return 0;
		}
	case WM_MBUTTONUP:
		{
			//Update position
			_mouse->SetX((int)LOWORD(lParam));
			_mouse->SetY((int)HIWORD(lParam));

			_mouse->SetMButton(false);

			return 0;
		}

	case WM_RBUTTONDOWN:
		{
			//Update position
			_mouse->SetX((int)LOWORD(lParam));
			_mouse->SetY((int)HIWORD(lParam));

			_mouse->SetRButton(true);

			return 0;
		}
	case WM_RBUTTONUP:
		{
			//Update position
			_mouse->SetX((int)LOWORD(lParam));
			_mouse->SetY((int)HIWORD(lParam));

			_mouse->SetRButton(false);

			return 0;
		}

	default:
		{
			return DefWindowProc(hWnd, umsg, wParam, lParam);
		}
	}
}


void System::InitWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	//Get an external pointer to this object;
	ApplicationHandle = this;

	//Get the instance of the application
	_hInstance = GetModuleHandle(NULL);

	//Give the application a name
	_applicationName = L"Engine";

	//Setup the windows class with default settings;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = _hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = _applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	//Register the window class
	RegisterClassEx(&wc);

	//Determine the resolution of the desktop screen
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	//Setup the screen settings depending on whether it is running in full screen or in windows mode
	if (EngineSettings::GetInstance().GetFullScreen())
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
		screenWidth = EngineSettings::GetInstance().GetWindowX();
		screenHeight = EngineSettings::GetInstance().GetWindowY();

		//Place the window in the middle of the screen
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth)/2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight)/2;
	}

	//Create the window with the screen settings and get the handle to it
	_hWnd = CreateWindowEx(WS_EX_APPWINDOW, _applicationName, _applicationName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW, posX, posY, screenWidth, screenHeight, NULL, NULL, _hInstance, NULL);

	//Bring the window up on the screen and set it as the main focus
	ShowWindow(_hWnd, SW_SHOW);
	SetForegroundWindow(_hWnd);
	SetFocus(_hWnd);

	//Hide the mouse cursor
	ShowCursor(true);

	EngineSettings::GetInstance().SetCurrentWindowX(screenWidth);
	EngineSettings::GetInstance().SetCurrentWindowY(screenHeight);
}

void System::ShutdownWindows()
{
	//Show the mouse cursor
	ShowCursor(true);

	//Fix the display settings if leaving full screen mode
	if (EngineSettings::GetInstance().GetFullScreen())
	{
		ChangeDisplaySettings(NULL,0);
	}

	//Remove the window
	DestroyWindow(_hWnd);
	_hWnd = NULL;

	//Remove the application instance
	UnregisterClass(_applicationName, _hInstance);
	_hInstance = NULL;

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

		//Check if the window is being closed
	case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		//All other messages pass to the message handler in the system class
	default:
		{
			return ApplicationHandle->MessageHandler(hWnd, umsg, wParam, lParam);
		}
	}
}

