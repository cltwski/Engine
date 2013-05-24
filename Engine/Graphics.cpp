#include "Graphics.h"

Graphics::Graphics()
{
	_d3d = NULL;
	_camera = NULL;
	_model = NULL;
	_bitmap = NULL;
	_textureShader = NULL;
	_text1 = NULL;
	_text2 = NULL;
	_textFps = NULL;
	_textCpu = NULL;
}

Graphics::Graphics(const Graphics& other)
{}

Graphics::~Graphics()
{}

bool Graphics::Init(int screenWidth, int screenHeight, HWND hWnd)
{
	bool result;
	D3DXMATRIX baseViewMatrix;

	//Create the Direct3D Object
	_d3d = new D3D();
	if (!_d3d)
		return false;

	//init the D3D object
	result = _d3d->Init(screenWidth, screenHeight, EngineSettings::GetInstance().GetVsync(), hWnd, EngineSettings::GetInstance().GetFullScreen(), SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hWnd, L"Could not init Direct3D", L"Error", MB_OK);
		return false;
	}

	//Create the new camera object
	_camera = new Camera();
	if (!_camera)
		return false;

	//Set the initial position of the camera
	_camera->SetPosition(0.0f, 0.0f, -1.0f);
	_camera->Render();
	_camera->GetViewMatrix(baseViewMatrix);


	//Init the two text objects
	_text1 = new Text();
	if (!_text1)
		return false;

	result = _text1->Init(_d3d->GetDevice(), _d3d->GetDeviceContext(), hWnd, screenWidth, screenHeight, baseViewMatrix,"", 20, 20);
	if (!result)
	{
		MessageBox(hWnd, L"Could not init the text object", L"Error", MB_OK);
		return false;
	}

	_text2 = new Text();
	if (!_text2)
		return false;

	result = _text2->Init(_d3d->GetDevice(), _d3d->GetDeviceContext(), hWnd, screenWidth, screenHeight, baseViewMatrix, "", 20, 40);
	if (!result)
	{
		MessageBox(hWnd, L"Could not init the second text object", L"Error", MB_OK);
		return false;
	}

	_textFps = new Text();
	if (!_textFps)
		return false;

	result = _textFps->Init(_d3d->GetDevice(), _d3d->GetDeviceContext(), hWnd, screenWidth, screenHeight, baseViewMatrix, "", screenWidth-100, 20);
	if (!result)
		return false;

	_textCpu = new Text();
	if (!result)
		return false;

	result = _textCpu->Init(_d3d->GetDevice(), _d3d->GetDeviceContext(), hWnd, screenWidth, screenHeight, baseViewMatrix, "", screenWidth-100, 40);
	if (!result)
		return false;

	return true;
}

void Graphics::Shutdown()
{
	if (_text1)
	{
		_text2->Shutdown();
		delete _text2;
		_text2 = NULL;
	}

	if (_text2)
	{
		_text2->Shutdown();
		delete _text2;
		_text2 = NULL;
	}

	if (_textFps)
	{
		_textFps->Shutdown();
		delete _textFps;
		_textFps = NULL;
	}

	if (_textCpu)
	{
		_textCpu->Shutdown();
		delete _textCpu;
		_textFps = NULL;
	}

	if (_camera)
	{
		delete _camera;
		_camera = NULL;
	}

	if (_d3d)
	{
		_d3d->Shutdown();
		delete _d3d;
		_d3d = NULL;
	}
}

bool Graphics::Frame(int mouseX, int mouseY, int fps, int cpu, float timer)
{
	bool result;

	char tempString[16];
	char mouseString[16];

	_itoa_s(mouseX, tempString, 10);
	strcpy_s(mouseString, "Mouse X:");
	strcat_s(mouseString, tempString);
	result = _text1->UpdateSentence(mouseString, _d3d->GetDeviceContext());
	if (!result)
		return false;

	_itoa_s(mouseY, tempString, 10);
	strcpy_s(mouseString, "Mouse Y:");
	strcat_s(mouseString, tempString);
	result = _text2->UpdateSentence(mouseString, _d3d->GetDeviceContext());
	if (!result)
		return false;

	char fpsString[32];
	float r,g,b;

	if (fps > 9999)
		fps = 9999;

	_itoa_s(fps, tempString, 10);
	strcpy_s(fpsString, "Fps: ");
	strcat_s(fpsString, tempString);

	if (fps >= 60)
	{
		r = 0.0f;
		g = 1.0f;
		b = 0.0f;
	}

	if (fps < 60 && fps >= 30)
	{
		r = 1.0f;
		g = 1.0f;
		b = 0.0f;
	}
	else if (fps < 30)
	{
		r = 1.0f;
		g = 0.0f;
		b = 0.0f;
	}

	result = _textFps->UpdateSentence(fpsString, r, g, b, _d3d->GetDeviceContext());
	if (!result)
		return false;

	char cpuString[32];
	
	_itoa_s(cpu, tempString, 10);
	strcpy_s(cpuString, "Cpu: ");
	strcat_s(cpuString, tempString);
	strcat_s(cpuString, "%");

	result = _textCpu->UpdateSentence(cpuString, r, g, b, _d3d->GetDeviceContext());
	if (!result)
		return false;

	//Set the position of the camera
	_camera->SetPosition(0.0f, 0.0f, -10.0f);

	return true;
}

bool Graphics::Render()
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix, orthoMatrix;
	bool result;

	//Clear the buffer to begin the scene, rgba clear color params
	_d3d->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	//Generate the view matrix based on the camera's position
	_camera->Render();

	//Get the world, view and projection matrices from the camera and d3d objects
	_camera->GetViewMatrix(viewMatrix);
	_d3d->GetWorldMatrix(worldMatrix);
	_d3d->GetProjectionMatrix(projectionMatrix);

	_d3d->GetOrthoMatrix(orthoMatrix);

	//Turn off the Z buffer to begin all 2D rendering
	_d3d->DisableZBuffer();

	//Turn on alpha blending before rendering the text
	_d3d->EnableAlphaBlending();

	//Render the text strings
	result = _text1->Render(_d3d->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
		return false;

	result = _text2->Render(_d3d->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
		return false;

	result = _textFps->Render(_d3d->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
		return false;

	result = _textCpu->Render(_d3d->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
		return false;

	//Turn off alpha blending after rendering the text
	_d3d->DisableAlphaBlending();

	//Re-enable Z buffer as we're done with 2d rendering
	_d3d->EnableZBuffer();

	//Present the rendered scene to the screen
	_d3d->EndScene();

	return true;
}