#pragma once

#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>

struct ShaderParams
{
	ID3D11DeviceContext* deviceContext;
	D3DXMATRIX worldMatrix;
	D3DXMATRIX viewMatrix;
	D3DXMATRIX projectionMatrix;
	ID3D11ShaderResourceView* texture;
	D3DXVECTOR4 pixelColor;
};
