#ifndef GRAPHICSCLASS_H
#define GRAPHICSCLASS_H

#include <Windows.h>

const bool FULL_SCREEN = true;
const bool VSYNC_ENABLED = true;
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
	bool Render();

};

#endif