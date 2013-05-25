#pragma once

#include <D3D11.h>
#include <unordered_map>

#include "Texture.h"

class TextureManager
{
public:
	static TextureManager& GetInstance();
	void Init(ID3D11Device*);
	void Shutdown();

	bool AddTexture(WCHAR*, const char*);
	Texture* GetTexture(const char*);
	void RemoveTexture(const char*);

private:
	TextureManager();
	TextureManager(const TextureManager&);
	void operator=(const TextureManager&);

	ID3D11Device* _device;
	std::unordered_map<const char*, Texture> _textures;
};