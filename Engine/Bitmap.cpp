#include "Bitmap.h"

Bitmap::Bitmap()
{
	_vertexBuffer = NULL;
	_indexBuffer = NULL;
	_texture = NULL;
}

Bitmap::Bitmap(const Bitmap& other)
{}

Bitmap::~Bitmap()
{}

bool Bitmap::Init(ID3D11Device* device, int screenWidth, int screenHeight, WCHAR* textureFilename, int bitmapWidth, int bitmapHeight)
{
	bool result;

	//Store the screen size
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;

	//Store the size in pixels that this bitmap should be rendered at
	_bitmapWidth = bitmapWidth;
	_bitmapHeight = bitmapHeight;

	//Init the prefvious rendering pos to a negative one
	_previousPosX = -1;
	_previousPosY = -1;

	//Init the vertex and index bufferes
	result = InitBuffers(device);
	if (!result)
		return false;

	//Load the texture for this 2d model
	result = LoadTexture(device, textureFilename);
	if (!result)
		return false;

	return true;
}

void Bitmap::Shutdown()
{
	//Release the texture
	ReleaseTexture();

	//Shutdown the vertex and index buffers
	ShutdownBuffers();
}

bool Bitmap::Render(ID3D11DeviceContext* deviceContext, int posX, int posY)
{
	bool result;

	//Rebuild the dynamic vertex buffer for rendering to possibly a different location on the screen
	result = UpdateBuffers(deviceContext, posX, posY);
	if (!result)
		return false;

	//Put the vertex and index buffers of the graphics pipeline to prepare them for drawing
	RenderBuffers(deviceContext);

	return true;
}

int Bitmap::GetIndexCount()
{
	return _indexCount;
}

ID3D11ShaderResourceView* Bitmap::GetTexture()
{
	return _texture->GetTexture();
}

bool Bitmap::InitBuffers(ID3D11Device* device)
{
	VertexTextureType* vertices;
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
	vertices = new VertexTextureType[_vertexCount];
	if (!vertices)
		return false;
	indices = new unsigned long[_indexCount];
	if (!indices)
		return false;

	//Init the vertex array to 0s at first
	memset(vertices, 0, (sizeof(VertexTextureType)*_vertexCount));

	//Load the index array with data
	for (i=0; i < _indexCount; ++i)
		indices[i] = i;

	// Set up the description of the dynamic vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexTextureType) * _vertexCount;
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

void Bitmap::ShutdownBuffers()
{
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

bool Bitmap::UpdateBuffers(ID3D11DeviceContext* deviceContext, int posX, int posY)
{
	float right,left,top,bottom;
	VertexTextureType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexTextureType* verticesPtr;
	HRESULT result;

	//Check if the position has changed. if it has not then we don't update the vertex buffer
	if ((posX == _previousPosX) && (posY == _previousPosY))
		return true;

	//If it has changed then we continue...
	//record the new location
	_previousPosX = posX;
	_previousPosY = posY;

	//Calculate the screen coords of the sides of the bitmap
	left = (float)((_screenWidth / 2) * -1) + (float)posX;
	right = left + (float)_bitmapWidth;
	top = (float)(_screenHeight/2) - (float)posY;
	bottom = top - (float)_bitmapHeight;

	//Create a temp vertex array
	vertices = new VertexTextureType[_vertexCount];
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
	verticesPtr = (VertexTextureType*)mappedResource.pData;

	//Copy the data into the vertex buffer
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexTextureType) * _vertexCount));

	//Unlock the vertex buffer
	deviceContext->Unmap(_vertexBuffer, 0);

	//delete the no longer needed resources
	delete[] vertices;
	vertices = NULL;

	return true;
}

void Bitmap::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	//Set the vertex buffer stride and offset
	stride = sizeof(VertexTextureType);
	offset = 0;

	//Set the vertex buffer to active in the input assembler so it can be rendered
	deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);

	//Set the index buffer to active in the input assembler so it can be rendered
	deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//Set the type of primitive that should be rendered from this vertex buffer, in this case triangles
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool Bitmap::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;

	_texture = new Texture();
	if (!_texture)
		return false;

	result = _texture->Init(device, filename);
	if (!result)
		return false;

	return true;
}

void Bitmap::ReleaseTexture()
{
	if (_texture)
	{
		_texture->Shutdown();
		delete _texture;
		_texture = 0;
	}
}