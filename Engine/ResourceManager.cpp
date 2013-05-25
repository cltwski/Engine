#include "ResourceManager.h"

ResourceManager::ResourceManager()
{}

ResourceManager& ResourceManager::GetInstance()
{
	static ResourceManager instance;
	return instance;
}

void ResourceManager::Init(ID3D11Device* device)
{
	TextureManager::GetInstance().Init(device);
	ShaderManager::GetInstance().Init(device);
	FontManager::GetInstance().Init(device);
}

void ResourceManager::Shutdown()
{
	FontManager::GetInstance().Shutdown();
	ShaderManager::GetInstance().Shutdown();
	FontManager::GetInstance().Shutdown();
}