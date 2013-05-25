#pragma once

#include "Shader.h"

class ColorShader : public Shader
{
public:
	ColorShader(void);
	ColorShader(const ColorShader&);
	~ColorShader(void);

private:
	bool InitShader(ID3D11Device*, HWND);
	void ShutdownShader();
	bool SetShaderParameters(ShaderParams);
	void RenderShader(ID3D11DeviceContext*, int);
};
