#pragma once

#include <D3D11.h>

#include "FontManager.h"
#include "TextureManager.h"
#include "ShaderManager.h"

class GraphicsResourceManager
{
public:
	static GraphicsResourceManager& GetInstance();

	void Init(ID3D11Device*);
	void Shutdown();

private:
	GraphicsResourceManager();
	GraphicsResourceManager(const GraphicsResourceManager&);
	void operator=(const GraphicsResourceManager&);
};