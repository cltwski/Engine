#pragma once

#include <D3D11.h>
#include <D3DX10math.h>

class IRenderable
{
public:
	virtual bool Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX)=0;

};