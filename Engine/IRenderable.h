#pragma once

#include <D3D11.h>

class IRenderable
{
public:
	virtual bool Render(ID3D11DeviceContext*)=0;

};