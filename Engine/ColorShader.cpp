#include "ColorShader.h"


ColorShader::ColorShader(void)
{
	Shader::Shader();
	_vsFilename = L"Data/Shaders/Vertex Shaders/ColorVS.hlsl";
	_psFilename = L"Data/Shaders/Pixel Shaders/ColorPS.hlsl";
}

ColorShader::ColorShader(const ColorShader& other)
{
	Shader::Shader(other);
}

ColorShader::~ColorShader(void)
{}

bool ColorShader::InitShader(ID3D11Device* device, HWND hwnd)
{
	Shader::InitShader(device, hwnd);
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



