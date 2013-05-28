#pragma once

#include "EngineSettings.h"

#include "Irenderable.h"

#include <D3DX11.h>
#include <D3DX10math.h>

#include "TextureManager.h"
#include "ShaderManager.h"

class Object2D : public IRenderable
{
protected:
	struct VertexTextureT
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	Object2D(void);
	Object2D(const Object2D&);
	~Object2D(void);

	bool Init(ID3D11Device*, char*, char*, int, int);
	bool SetPosition(ID3D11DeviceContext*, int, int);
	bool Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();
	int GetIndexCount();

protected:
	ID3D11Buffer* _vertexBuffer;
	ID3D11Buffer* _indexBuffer;
	int _vertexCount;
	int _indexCount;
	Texture* _texture;
	Shader* _shader;
	GUID _guid;

	int _width, _height;
	int _prevPosX, _prevPosY;
	int _posX, _posY;

	bool InitBuffers(ID3D11Device*);
	bool UpdateBuffers(ID3D11DeviceContext*, int, int);
};

