#include "Graphics.h"

Graphics::Graphics()
{
	_d3d = NULL;
	_camera = NULL;
	_model = NULL;
	_text1 = NULL;
	_text2 = NULL;
	_splash = NULL;
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

	//Init all graphics resources.
	GraphicsResourceManager::GetInstance().Init(_d3d->GetDevice());

	//Load all assets
	result = LoadAssets(hWnd);
	if (!result)
		return false;

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

	for (int i=0; i < 4; ++i)
	{
		_objects2d.push_back(new Object2D());
		result = _objects2d[i]->Init(_d3d->GetDevice(), "Grass", "Texture", 64, 64);
		if (!result) 
			return false;
	}

	for (int i=0; i < 4; ++i)
	{
		_objects2d.push_back(new Object2D());
		result = _objects2d[i+2]->Init(_d3d->GetDevice(), "Water", "Texture", 64, 64);
		if (!result) 
			return false;
	}

	_splash = new Object2D();
	_splash->Init(_d3d->GetDevice(), "SplashScreen", "Texture", EngineSettings::GetInstance().GetCurrentWindowX(), EngineSettings::GetInstance().GetCurrentWindowY());

	return true;
}

bool Graphics::LoadAssets(HWND hwnd)
{
	bool result;

	//Load textures first
	result = TextureManager::GetInstance().AddTexture(L"Data/Fonts/font.dds", "StandardFont");
	if (!result)
		return false;
	result = TextureManager::GetInstance().AddTexture(L"Data/Textures/Grass.dds", "Grass");
	if (!result)
		return false;
	result = TextureManager::GetInstance().AddTexture(L"Data/Textures/Water.dds", "Water");
	if (!result)
		return false;
	result = TextureManager::GetInstance().AddTexture(L"Data/Textures/SplashScreen.dds", "SplashScreen");
	if (!result)
		return false;
	result = TextureManager::GetInstance().AddTexture(L"Data/Textures/Button.dds", "Button");

	//Load shaders
	result = ShaderManager::GetInstance().AddShader(new TextureShader(), hwnd, "Texture");
	if (!result)
		return false;
	result = ShaderManager::GetInstance().AddShader(new FontShader(), hwnd, "Font");
	if (!result)
		return false;
	

	//Load fonts
	result = FontManager::GetInstance().AddFont("Data/Fonts/fontdata.txt", "StandardFont", "Standard");
	if (!result)
		return false;

	return true;
}


void Graphics::Shutdown()
{
	for (int i=0; i < _objects2d.size(); ++i)
	{
		_objects2d[i]->Shutdown();
		delete _objects2d[i];
		_objects2d[i] = NULL;
	}
	_objects2d.clear();

	if (_splash)
	{
		_splash = NULL;
	}

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

	_objects2d[0]->SetPosition(_d3d->GetDeviceContext(), 100+mouseX, 100+mouseY);
	_objects2d[1]->SetPosition(_d3d->GetDeviceContext(), 164+mouseX, 164+mouseY);
	_objects2d[2]->SetPosition(_d3d->GetDeviceContext(), 100+mouseX, 164+mouseY);
	_objects2d[3]->SetPosition(_d3d->GetDeviceContext(), 164+mouseX, 100+mouseY);

	return true;
}

bool Graphics::FrameSplash()
{
	_splash->SetPosition(_d3d->GetDeviceContext(), 0, 0);

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
	result = _text1->Render(_d3d->GetDeviceContext(), worldMatrix, viewMatrix, orthoMatrix);
	if (!result)
		return false;

	result = _text2->Render(_d3d->GetDeviceContext(), worldMatrix, viewMatrix, orthoMatrix);
	if (!result)
		return false;

	//Render non alpha blending 2d
	for (int i=0; i < _objects2d.size(); ++i)
	{
		_objects2d[i]->Render(_d3d->GetDeviceContext(), worldMatrix, viewMatrix, orthoMatrix);
	}

	//Turn off alpha blending after rendering the text
	_d3d->DisableAlphaBlending();

	//Re-enable Z buffer as we're done with 2d rendering
	_d3d->EnableZBuffer();

	//Present the rendered scene to the screen
	_d3d->EndScene();

	return true;
}

bool Graphics::RenderSplash()
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

	_splash->Render(_d3d->GetDeviceContext(), worldMatrix, viewMatrix, orthoMatrix);

	//Turn off alpha blending after rendering the text
	_d3d->DisableAlphaBlending();

	//Re-enable Z buffer as we're done with 2d rendering
	_d3d->EnableZBuffer();

	//Present the rendered scene to the screen
	_d3d->EndScene();

	return true;
}