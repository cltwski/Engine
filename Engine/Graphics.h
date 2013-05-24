#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "EngineSettings.h"
#include "D3D.h"
#include "Camera.h"
#include "Model.h"
#include "Bitmap.h"
#include "TextureShader.h"

const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class Graphics
{
public:
	Graphics();
	Graphics(const Graphics&);
	~Graphics();

	bool Init(int,int,HWND);
	void Shutdown();
	bool Frame();

private:
	D3D* _d3d;
	Camera* _camera;
	Model* _model;
	Bitmap* _bitmap;
	TextureShader* _textureShader;

	bool Render();

};

#endif