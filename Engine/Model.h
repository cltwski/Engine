#ifndef MODEL_H
#define MODEL_H

#include <D3D11.h>
#include <D3DX10math.h>

#include "Texture.h"

class Model
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	Model();
	Model(const Model&);
	~Model();

	bool Init(ID3D11Device*, WCHAR*);
	void Shutdown();

	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	ID3D11Buffer* _vertexBufferPtr;
	ID3D11Buffer* _indexBufferPtr;
	int _vertexCount;
	int _indexCount;
	Texture* _texture;

	bool InitBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();
};

#endif