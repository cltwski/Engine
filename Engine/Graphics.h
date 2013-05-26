#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "EngineSettings.h"
#include "D3D.h"
#include "Camera.h"
#include "Model.h"
#include "Bitmap.h"
#include "Text.h"
#include "FontManager.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "GraphicsResourceManager.h"

const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class FontShader;

class Graphics
{
public:
	Graphics();
	Graphics(const Graphics&);
	~Graphics();

	bool Init(int,int,HWND);
	void Shutdown();
	bool Frame(int, int, int, int, float);
	bool Render();

private:
	D3D* _d3d;
	Camera* _camera;
	Model* _model;
	Bitmap* _bitmap;
	Text* _text1;
	Text* _text2;

	bool LoadAssets(HWND);
};

#endif