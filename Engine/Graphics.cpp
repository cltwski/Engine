#include "Graphics.h"

Graphics::Graphics()
{
	_d3d = NULL;
	_camera = NULL;
	_model = NULL;
	_bitmap = NULL;
	_text1 = NULL;
	_text2 = NULL;
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

	//Init Font texture
	TextureManager::GetInstance().Init(_d3d->GetDevice());
	result = TextureManager::GetInstance().AddTexture(L"Data/Fonts/font.dds", "StandardFont");
	if (!result)
	{
		MessageBox(hWnd, L"Could not add texture", L"Error", MB_OK);
		return false;
	}

	//Init the font shader
	ShaderManager::GetInstance().Init(_d3d->GetDevice());
	result = ShaderManager::GetInstance().AddShader(new FontShader(), hWnd, "Font");
	if (!result)
	{
		MessageBox(hWnd, L"Could not add shader", L"Error", MB_OK);
		return false;
	}

	//Init the font
	FontManager::GetInstance().Init(_d3d->GetDevice());
	result = FontManager::GetInstance().AddFont("Data/Fonts/fontdata.txt", "StandardFont", "Standard");
	if (!result)
	{
		MessageBox(hWnd, L"Could not add font", L"Error", MB_OK);
		return false;
	}

	//Init the text
	_text1 = new Text();
	if (!_text1)
		return false;

	result = _text1->Init(_d3d->GetDevice(), _d3d->GetDeviceContext(), hWnd, "Standard", "Font", baseViewMatrix, "", 20, 20, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		MessageBox(hWnd, L"Could not init text1", L"Error", MB_OK);
		return false;
	}

	//Init the text
	_text2 = new Text();
	if (!_text2)
		return false;

	result = _text2->Init(_d3d->GetDevice(), _d3d->GetDeviceContext(), hWnd, "Standard", "Font", baseViewMatrix, "", 20, 40, 1.0f, 1.0f, 1.0f);
	if (!result)
	{
		MessageBox(hWnd, L"Could not init text2", L"Error", MB_OK);
		return false;
	}

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
	result = _text1->UpdateWords(mouseString, _d3d->GetDeviceContext());
	if (!result)
		return false;

	_itoa_s(mouseY, tempString, 10);
	strcpy_s(mouseString, "Mouse Y:");
	strcat_s(mouseString, tempString);
	result = _text2->UpdateWords(mouseString, _d3d->GetDeviceContext());
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

	//Turn off alpha blending after rendering the text
	_d3d->DisableAlphaBlending();

	//Re-enable Z buffer as we're done with 2d rendering
	_d3d->EnableZBuffer();

	//Present the rendered scene to the screen
	_d3d->EndScene();

	return true;
}