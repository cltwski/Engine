#include "Model.h"

Model::Model()
{
	_vertexBufferPtr = NULL;
	_indexBufferPtr = NULL;
}

Model::Model(const Model& other)
{}

Model::~Model()
{}

bool Model::Init(ID3D11Device* device)
{
	bool result;

	//Init the vertex and index buffers that hold the geometry for the triangle
	result = InitBuffers(device);
	if (!result)
		return false;

	return true;
}

void Model::Shutdown()
{
	//Release the vertex and index buffers
	ShutdownBuffers();
}

void Model::Render(ID3D11DeviceContext* deviceContext)
{
	//Put the vertex and index buffers on the graphics pipeline to prepare them for drawing
	RenderBuffers(deviceContext);
}

int Model::GetIndexCount()
{
	return _indexCount;
}

bool Model::InitBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	//Set the number of vertices in the vertex array
	_vertexCount = 6;

	//Set the number of indices
	_indexCount = 6;

	//Create the vertex array
	vertices = new VertexType[_vertexCount];
	if (!vertices)
		return false;

	//Create the index array
	indices = new unsigned long[_indexCount];
	if (!indices)
		return false;

	//Load the vertex array with data
	vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f); //bottom left
	vertices[0].color = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[1].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f); //top left
	vertices[1].color = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[2].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f); //bottom right
	vertices[2].color = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[3].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f); //top left
	vertices[3].color = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[4].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f); //top right
	vertices[4].color = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[5].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f); //bottom right
	vertices[5].color = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);

	//Load the index array with data
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 3;
	indices[4] = 4;
	indices[5] = 5;

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * _vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBufferPtr);
	if (FAILED(result))
		return false;

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * _indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//Create the index buffer
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &_indexBufferPtr);
	if (FAILED(result))
		return false;

	delete[] vertices;
	vertices = NULL;

	delete[] indices;
	indices = NULL;

	return true;
}

void Model::ShutdownBuffers()
{
	//Release the index buffer
	if (_indexBufferPtr)
	{
		_indexBufferPtr->Release();
		_indexBufferPtr = NULL;
	}

	//Release the vertex buffer
	if (_vertexBufferPtr)
	{
		_vertexBufferPtr->Release();
		_vertexBufferPtr = NULL;
	}
}

void Model::RenderBuffers(ID3D11DeviceContext* deviceContext)

{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	//Set the vertex buffer to active in the input assembler so it can be rendered
	deviceContext->IASetVertexBuffers(0, 1, &_vertexBufferPtr, &stride, &offset);

	//Set the index buffer to active in the input assembler so it can be rendered
	deviceContext->IASetIndexBuffer(_indexBufferPtr, DXGI_FORMAT_R32_UINT, 0);

	//Set the type of primitive that should be rendered from this vertex buffer, in this case triangles
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}



