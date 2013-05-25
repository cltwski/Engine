#include "TextureShader.h"


TextureShader::TextureShader(void)
{
	Shader::Shader();
	_samplerState = NULL;
}

TextureShader::TextureShader(const TextureShader& other)
{
	Shader::Shader(other);
}

TextureShader::~TextureShader(void)
{}

bool TextureShader::InitShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	Shader::InitShader(device, hwnd, vsFilename, psFilename);
}

void TextureShader::ShutdownShader()
{
	//release the sampler state
	if (_samplerState)
	{
		_samplerState->Release();
		_samplerState = NULL;
	}

	Shader::ShutdownShader();
}

bool TextureShader::SetShaderParameters(ShaderParams params)
{
	Shader::SetShaderParameters(params);

	//Set the shader texture resource in the pixel shader
	params.deviceContext->PSSetShaderResources(0, 1, &params.texture);
}

void TextureShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	//Set the vertex input layout
	deviceContext->IASetInputLayout(_layout);

	//Set the vertex and pixel shaders that will be used to render this triangle
	deviceContext->VSSetShader(_vertexShader, NULL, 0);
	deviceContext->PSSetShader(_pixelShader, NULL, 0);

	//Set the sampler state in the pixel shader
	deviceContext->PSSetSamplers(0, 1, &_samplerState);

	//Render the triangle
	deviceContext->DrawIndexed(indexCount, 0, 0);
}

