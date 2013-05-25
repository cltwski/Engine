#include "GraphicsResourceManager.h"

GraphicsResourceManager::GraphicsResourceManager()
{}

GraphicsResourceManager& GraphicsResourceManager::GetInstance()
{
	static GraphicsResourceManager instance;
	return instance;
}

void GraphicsResourceManager::Init(ID3D11Device* device)
{
	TextureManager::GetInstance().Init(device);
	ShaderManager::GetInstance().Init(device);
	FontManager::GetInstance().Init(device);
}

void GraphicsResourceManager::Shutdown()
{
	FontManager::GetInstance().Shutdown();
	ShaderManager::GetInstance().Shutdown();
	FontManager::GetInstance().Shutdown();
}