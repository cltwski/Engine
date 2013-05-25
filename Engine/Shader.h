#ifndef SHADER_H
#define SHADER_H

#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>

#include "ShaderParams.h"

class Shader
{
protected:
	struct MatrixBufferT
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

public:
	Shader();
	Shader(const Shader&);
	~Shader();

	virtual bool Init(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ShaderParams, int);

	WCHAR* GetVsFilename();
	WCHAR* GetPsFilename();

protected:
	WCHAR* _vsFilename;
	WCHAR* _psFilename;
	ID3D11VertexShader* _vertexShader;
	ID3D11PixelShader* _pixelShader;
	ID3D11InputLayout* _layout;
	ID3D11Buffer* _matrixBuffer;

	virtual bool InitShader(ID3D11Device*, HWND);
	virtual void ShutdownShader();
	virtual bool SetShaderParameters(ShaderParams);
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);
	virtual void RenderShader(ID3D11DeviceContext*, int);
};

#endif