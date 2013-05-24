#include "Graphics.h"

Graphics::Graphics()
{
	_d3d = NULL;
	_camera = NULL;
	_model = NULL;
	_colorShader = NULL;
}

Graphics::Graphics(const Graphics& other)
{}

Graphics::~Graphics()
{}

bool Graphics::Init(int screenWidth, int screenHeight, HWND hWnd)
{
	bool result;

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
	_camera->SetPosition(0.0f, 0.0f, -20.0f);

	//Create the model object
	_model = new Model();
	if (!_model)
		return false;

	//Init the model
	result = _model->Init(_d3d->GetDevice());
	if (!result)
	{
		MessageBox(hWnd, L"Could not init the model object", L"Error", MB_OK);
		return false;
	}

	//Create the color shader object
	_colorShader = new ColorShader();
	if (!_colorShader)
		return false;

	//Init the color shader
	result = _colorShader->Init(_d3d->GetDevice(), hWnd);
	if (!result)
	{
		MessageBox(hWnd, L"Could not init the color shader object", L"Error", MB_OK);
		return false;
	}

	return true;
}

void Graphics::Shutdown()
{
	if (_colorShader)
	{
		_colorShader->Shutdown();
		delete _colorShader;
		_colorShader = NULL;
	}

	if (_model)
	{
		_model->Shutdown();
		delete _model;
		_model = NULL;
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

bool Graphics::Frame()
{
	bool result;

	//Render the graphics scene
	result = Render();

	if (!result)
		return false;

	return true;
}

bool Graphics::Render()
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
	bool result;

	//Clear the buffer to begin the scene, rgba clear color params
	_d3d->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	//Generate the view matrix based on the camera's position
	_camera->Render();

	//Get the world, view and projection matrices from the camera and d3d objects
	_camera->GetViewMatrix(viewMatrix);
	_d3d->GetWorldMatrix(worldMatrix);
	_d3d->GetProjectionMatrix(projectionMatrix);

	//Put the model vertex and index buffer on the graphics pipeline to prepare them for drawing
	_model->Render(_d3d->GetDeviceContext());

	//Render the model using the color shader
	result = _colorShader->Render(_d3d->GetDeviceContext(), _model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
		return false;

	//Present the rendered scene to the screen
	_d3d->EndScene();

	return true;
}