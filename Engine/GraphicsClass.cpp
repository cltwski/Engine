#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = NULL;
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{}

GraphicsClass::~GraphicsClass()
{}

bool GraphicsClass::Init(int screenWidth, int screenHeight, HWND hWnd)
{
	bool result;

	//Create the Direct3D Object
	m_D3D = new D3DClass();
	if (!m_D3D)
		return false;

	//init the D3D object
	result = m_D3D->Init(screenWidth, screenHeight, EngineSettings::GetInstance().GetVsync(), hWnd, EngineSettings::GetInstance().GetFullScreen(), SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hWnd, L"Could not init Direct3D", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = NULL;
	}
}

bool GraphicsClass::Frame()
{
	bool result;

	//Render the graphics scene
	result = Render();

	if (!result)
		return false;

	return true;
}

bool GraphicsClass::Render()
{
	//Clear the buffer to begin the scene, rgba clear color params
	m_D3D->BeginScene(1.0f, 1.0f, 0.0f, 1.0f);

	//Present the rendered scene to the screen
	m_D3D->EndScene();

	return true;
}