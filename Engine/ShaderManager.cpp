#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
	_device = NULL;
}

ShaderManager& ShaderManager::GetInstance()
{
	static ShaderManager instance;
	return instance;
}

void ShaderManager::Init(ID3D11Device* device)
{
	_device = device;
}

void ShaderManager::Shutdown()
{
	for (auto it = _shaders.begin(); it != _shaders.end(); ++it)
	{
		it->second->Shutdown();
	}
	_shaders.clear();
}

bool ShaderManager::AddShader(Shader* shader, HWND hwnd, const char* name)
{
	bool result;

	_shaders.insert(std::make_pair<const char*, Shader*>(name, shader));
	result = _shaders[name]->Init(_device, hwnd);
	if (!result)
		return false;

	return true;
}

Shader* ShaderManager::GetShader(const char* name)
{
	return _shaders[name];
}

void ShaderManager::RemoveShader(const char* name)
{
	_shaders.erase(name);
}
