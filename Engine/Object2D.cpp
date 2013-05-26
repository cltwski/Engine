#include "Object2D.h"


Object2D::Object2D(void)
{}

Object2D::Object2D(const Object2D& other)
{}

Object2D::~Object2D(void)
{}

bool Object2D::Init(ID3D11Device* device, char* textureName, int width, int height)
{
	bool result;
	HRESULT result2;

	result2 = CoCreateGuid(&_guid);
	if (FAILED(result2))
		return false;

	//Store the size in pixels that this bitmap should be rendered at
	_width = width;
	_height = height;

	//Init the current position
	_posX = -1;
	_posY = -1;

	//Init previous position
	_prevPosX = -1;
	_prevPosY = -1;

	//Init the vertex and index buffers
	result = InitBuffers(device);
	if (!result)
		return false;

	//Acquire the texture
	_texture = TextureManager::GetInstance().GetTexture(textureName);
	if (!_texture)
		return false;

	return true;
}

bool Object2D::SetPosition(ID3D11DeviceContext* deviceContext, int newPosX, int newPosY)
{
	if ((newPosX == _posX) && (newPosY == _posY))
		return;
	
	bool result;

	//Rebuild the dynamic vertex buffer to render to the new location
	result = UpdateBuffers(deviceContext, newPosX, newPosY);
	if (!result)
		return false;

	return true;
}

bool Object2D::Render(ID3D11DeviceContext* deviceContext)
{
	//Check that the position has been set
	if ((_posX == -1) || (_posY == -1))
		return false;

	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexTextureT);
	offset = 0;

	//Set the vertex buffer to active in the input assembler so it can be rendered
	deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);

	//Set the index buffer to active in the input assembler so it can be rendered
	deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//Set the type of primitive that should be rendered from this vertex buffer, in this case triangles
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}

void Object2D::Shutdown()
{
	_texture = NULL;

	if (_vertexBuffer)
	{
		_vertexBuffer->Release();
		_vertexBuffer = NULL;
	}

	if (_indexBuffer)
	{
		_indexBuffer->Release();
		_indexBuffer = NULL;
	}
}

bool Object2D::InitBuffers(ID3D11Device* device)
{
	VertexTextureT* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	//Set the number of vertices in the vertex array
	_vertexCount = 6;

	//Set the number of indices in the index array
	_indexCount = 6;

	//Create the arrays
	vertices = new VertexTextureT[_vertexCount];
	if (!vertices)
		return false;
	indices = new unsigned long[_indexCount];
	if (!indices)
		return false;

	//Init the vertex array to 0s at first
	memset(vertices, 0, (sizeof(VertexTextureT)*_vertexCount));

	//Load the index array with data
	for (i=0; i < _indexCount; ++i)
		indices[i] = i;

	// Set up the description of the dynamic vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexTextureT) * _vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
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

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer);
	if (FAILED(result))
		return false;

	//Release the arrays now that the buffers have been created and loaded
	delete[] vertices;
	vertices = NULL;

	delete[] indices;
	indices = NULL;

	return true;
}

bool Object2D::UpdateBuffers(ID3D11DeviceContext* deviceContext, int newPosX, int newPosY)
{
	float right,left,top,bottom;
	VertexTextureT* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexTextureT* verticesPtr;
	HRESULT result;

	//Update the positions
	_previousPosX = _posX;
	_previousPoxY = _posY;
	_posX = newPosX;
	_posY = newPosY;

	//Calculate the screen coords of the sides of the bitmap
	left = (float)((EngineSettings::GetInstance().GetCurrentWindowX() / 2) * -1) + (float)_posX;
	right = left + (float)_width;
	top = (float)(EngineSettings::GetInstance().GetCurrentWindowY() / 2) - (float)_posY;
	bottom = top - (float)_height;

	//Create a temp vertex array
	vertices = new VertexTextureT[_vertexCount];
	if (!vertices)
		return false;

	//Load the vertex array with data
	// First triangle.
	vertices[0].position = D3DXVECTOR3(left, top, 0.0f);  // Top left.
	vertices[0].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[1].position = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
	vertices[1].texture = D3DXVECTOR2(1.0f, 1.0f);

	vertices[2].position = D3DXVECTOR3(left, bottom, 0.0f);  // Bottom left.
	vertices[2].texture = D3DXVECTOR2(0.0f, 1.0f);

	// Second triangle.
	vertices[3].position = D3DXVECTOR3(left, top, 0.0f);  // Top left.
	vertices[3].texture = D3DXVECTOR2(0.0f, 0.0f);

	vertices[4].position = D3DXVECTOR3(right, top, 0.0f);  // Top right.
	vertices[4].texture = D3DXVECTOR2(1.0f, 0.0f);

	vertices[5].position = D3DXVECTOR3(right, bottom, 0.0f);  // Bottom right.
	vertices[5].texture = D3DXVECTOR2(1.0f, 1.0f);

	//Now copy the contents of the vertex array into the vertex buffer
	//Lock the vertex buffer so it can be written to
	result = deviceContext->Map(_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
		return false;

	//Get a pointer to the data in the vertex buffer
	verticesPtr = (VertexTextureT*)mappedResource.pData;

	//Copy the data into the vertex buffer
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexTextureT) * _vertexCount));

	//Unlock the vertex buffer
	deviceContext->Unmap(_vertexBuffer, 0);

	//delete the no longer needed resources
	delete[] vertices;
	vertices = NULL;

	return true;

	return true;
}
