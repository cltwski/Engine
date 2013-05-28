#pragma once

#include "Shader.h"

class TextureShader : public Shader
{
public:
	TextureShader(void);
	TextureShader(const TextureShader&);
	~TextureShader(void);

	bool Init(ID3D11Device*, HWND);

private:
	ID3D11SamplerState* _samplerState;

	bool InitShader(ID3D11Device*, HWND);
	void ShutdownShader();
	bool SetShaderParameters(ShaderParams);
	void RenderShader(ID3D11DeviceContext*, int);
};

