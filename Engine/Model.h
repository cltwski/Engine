#ifndef MODEL_H
#define MODEL_H

#include <D3D11.h>
#include <D3DX10math.h>

class Model
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR4 color;
	};

public:
	Model();
	Model(const Model&);
	~Model();

	bool Init(ID3D11Device*);
	void Shutdown();

	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	ID3D11Buffer* _vertexBufferPtr;
	ID3D11Buffer* _indexBufferPtr;
	int _vertexCount;
	int _indexCount;

	bool InitBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
};

#endif