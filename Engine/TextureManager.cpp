#include "TextureManager.h"

TextureManager::TextureManager()
{
	_device = NULL;
}

TextureManager& TextureManager::GetInstance()
{
	static TextureManager instance;
	return instance;
}

void TextureManager::Init(ID3D11Device* device)
{
	_device = device;
}

void TextureManager::Shutdown()
{
	for (auto it = _textures.begin(); it != _textures.end(); ++it)
	{
		it->second.Shutdown();
	}
	_textures.clear();
}

bool TextureManager::AddTexture(WCHAR* filename, const char* name)
{
	Texture texture;
	bool result;

	result = texture.Init(_device, filename);
	if (!result)
		return false;

	_textures.insert(std::make_pair<const char*,Texture>(name, texture));
	return true;
}

Texture* TextureManager::GetTexture(const char* name)
{
	return &_textures[name];
}

void TextureManager::RemoveTexture(const char* name)
{
	_textures.erase(name);
}