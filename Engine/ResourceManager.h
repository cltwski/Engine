#pragma once

#include <D3D11.h>

#include "FontManager.h"
#include "TextureManager.h"
#include "ShaderManager.h"

class ResourceManager
{
public:
	static ResourceManager& GetInstance();

	void Init(ID3D11Device*);
	void Shutdown();

private:
	ResourceManager();
	ResourceManager(const ResourceManager&);
	void operator=(const ResourceManager&);
};