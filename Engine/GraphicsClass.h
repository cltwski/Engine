#ifndef GRAPHICSCLASS_H
#define GRAPHICSCLASS_H

#include "EngineSettings.h"
#include "D3DClass.h"

const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Init(int,int,HWND);
	void Shutdown();
	bool Frame();

private:
	D3DClass* m_D3D;

	bool Render();

};

#endif