#pragma once

#include "Shader.h"

class FontShader : public Shader
{
private:
	struct PixelBufferT
	{
		D3DXVECTOR4 pixelColor;
	};

public:
	FontShader(void);
	FontShader(const FontShader&);
	~FontShader(void);

private:
	ID3D11SamplerState* _samplerState;
	ID3D11Buffer* _pixelBuffer;

	bool InitShader(ID3D11Device*, HWND);
	void ShutdownShader();
	bool SetShaderParameters(ShaderParams);
	void RenderShader(ID3D11DeviceContext*, int);
};

