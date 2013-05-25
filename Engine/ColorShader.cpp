#include "ColorShader.h"


ColorShader::ColorShader(void)
{
	Shader::Shader();
}

ColorShader::ColorShader(const ColorShader& other)
{
	Shader::Shader(other);
}


ColorShader::~ColorShader(void)
{}

bool ColorShader::InitShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	Shader::InitShader(device, hwnd, vsFilename, psFilename);
}

void ColorShader::ShutdownShader()
{
	Shader::ShutdownShader();
}

bool ColorShader::SetShaderParameters(ShaderParams params)
{
	Shader::SetShaderParameters(params);
}

void ColorShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	Shader::RenderShader(deviceContext, indexCount);
}



