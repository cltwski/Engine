#pragma once

#include <D3D11.h>
#include <unordered_map>

#include "Shader.h"
#include "FontShader.h"
#include "ColorShader.h"
#include "TextureShader.h"

class ShaderManager
{
public:
	static ShaderManager& GetInstance();
	void Init(ID3D11Device*);
	void Shutdown();
	
	bool AddShader(Shader*, HWND, const char*);
	Shader* GetShader(const char*);
	void RemoveShader(const char*);

private:
	ShaderManager();
	ShaderManager(const ShaderManager&);
	void operator=(const ShaderManager&);

	ID3D11Device* _device;
	std::unordered_map<const char*, Shader*> _shaders;
};