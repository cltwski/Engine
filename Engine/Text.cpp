#include "Text.h"

Text::Text()
{
	_font = NULL;
	_shader = NULL;
	_sentence = NULL;
}

Text::Text(const Text& other)
{}

Text::~Text()
{}

bool Text::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, char* fontName, char* shaderName, D3DXMATRIX baseViewMatrix, char* words, int startX, int startY, float startR, float startG, float startB)
{
	bool result;
	HRESULT result2;

	//Set protected variables
	_r = startR;
	_g = startG;
	_b = startB;
	_words = words;
	_posX = startX;
	_posY = startY;
	_baseViewMatrix = baseViewMatrix;
	_drawX = 0;
	_drawY = 0;

	//Init Guid
	result2 = CoCreateGuid(&_guid);
	if (FAILED(result2))
		return false;

	//Get font
	_font = FontManager::GetInstance().GetFont(fontName);

	//Get shader
	_shader = ShaderManager::GetInstance().GetShader(shaderName);

	//Init the sentence
	result = InitSentence(&_sentence, 16, device);
	if (!result)
		return false;

	//Update the sentence
	result = UpdateSentence(_words, startX, startY, startR, startG, startB, deviceContext);
	if (!result)
		return false;

	return true;
}

void Text::SetMatrices(D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix)
{
	_worldMatrix = worldMatrix;
	_orthoMatrix = orthoMatrix;
}

bool Text::Render(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride, offset;
	D3DXVECTOR4 pixelColor;
	bool result;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexTextureT); 
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &_sentence->vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(_sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create a pixel color vector with the input sentence color.
	pixelColor = D3DXVECTOR4(_sentence->red, _sentence->green, _sentence->blue, 1.0f);

	// Render the text using the font shader.
	ShaderParams params;
	params.deviceContext = deviceContext;
	params.pixelColor = pixelColor;
	params.worldMatrix = _worldMatrix;
	params.viewMatrix = _baseViewMatrix;
	params.projectionMatrix = _orthoMatrix;
	params.texture = _font->GetTexture();
	result = _shader->Render(params, _sentence->indexCount);
	if(!result)
	{
		false;
	}

	return true;
}

void Text::Shutdown()
{
	if(_sentence)
	{
		// Release the sentence vertex buffer.
		if(_sentence->vertexBuffer)
		{
			_sentence->vertexBuffer->Release();
			_sentence->vertexBuffer = NULL;
		}

		// Release the sentence index buffer.
		if(_sentence->indexBuffer)
		{
			_sentence->indexBuffer->Release();
			_sentence->indexBuffer = NULL;
		}

		// Release the sentence.
		delete _sentence;
		_sentence = NULL;
	}
}

bool Text::UpdateWords(char* newWords, ID3D11DeviceContext* deviceContext)
{
	int numLetters;
	VertexTextureT* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result;
	VertexTextureT* verticesPtr;

	//Get the number of letters in the sentence
	numLetters = (int)strlen(newWords);

	// Check for possible buffer overflow.
	if (numLetters > _sentence->maxLength)
		return false;

	//Update the internal words
	_words = newWords;

	// Create the vertex array.
	vertices = new VertexTextureT[_sentence->vertexCount];
	if(!vertices)
		return false;

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexTextureT) * _sentence->vertexCount));

	// Use the font class to build the vertex array from the sentence text and sentence draw location.
	_font->BuildVertexArray((void*)vertices, newWords, _drawX, _drawY);

	// Lock the vertex buffer so it can be written to.
	result = deviceContext->Map(_sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
		return false;

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (VertexTextureT*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexTextureT) * _sentence->vertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(_sentence->vertexBuffer, 0);

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	return true;
}

bool Text::UpdateColor(float red, float green, float blue, ID3D11DeviceContext* deviceContext)
{
	HRESULT result;
	VertexTextureT* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexTextureT* verticesPtr;

	//Store the color
	_sentence->red = red;
	_sentence->green = green;
	_sentence->blue = blue;
	_r = red;
	_g = green;
	_b = blue;

	// Create the vertex array.
	vertices = new VertexTextureT[_sentence->vertexCount];
	if(!vertices)
		return false;

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexTextureT) * _sentence->vertexCount));

	// Use the font class to build the vertex array from the sentence text and sentence draw location.
	_font->BuildVertexArray((void*)vertices, _words, _drawX, _drawY);

	// Lock the vertex buffer so it can be written to.
	result = deviceContext->Map(_sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
		return false;

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (VertexTextureT*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexTextureT) * _sentence->vertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(_sentence->vertexBuffer, 0);

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	return true;
}

bool Text::UpdatePosition(int newPosX, int newPosY, ID3D11DeviceContext* deviceContext)
{
	VertexTextureT* vertices;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexTextureT* verticesPtr;

	// Create the vertex array.
	vertices = new VertexTextureT[_sentence->vertexCount];
	if(!vertices)
		return false;

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexTextureT) * _sentence->vertexCount));

	//Update the internal variable
	_posX = newPosX;
	_posY = newPosY;

	// Calculate the X and Y pixel position on the screen to start drawing to.
	_drawX = (float)(((EngineSettings::GetInstance().GetCurrentWindowX() / 2) * -1) + newPosX);
	_drawY = (float)((EngineSettings::GetInstance().GetCurrentWindowY() / 2) - newPosY);

	// Use the font class to build the vertex array from the sentence text and sentence draw location.
	_font->BuildVertexArray((void*)vertices, _words, _drawX, _drawY);

	// Lock the vertex buffer so it can be written to.
	result = deviceContext->Map(_sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
		return false;

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (VertexTextureT*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexTextureT) * _sentence->vertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(_sentence->vertexBuffer, 0);

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	return true;
}

bool Text::UpdateSentence(char* newWords, int newPosX, int newPosY, float newR, float newG, float newB, ID3D11DeviceContext* deviceContext)
{
	int numLetters;
	VertexTextureT* vertices;
	float drawX, drawY;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexTextureT* verticesPtr;

	//Store the color
	_sentence->red = newR;
	_sentence->green = newG;
	_sentence->blue = newR;
	_r = newR;
	_g = newG;
	_b = newR;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(newWords);

	// Check for possible buffer overflow.
	if(numLetters > _sentence->maxLength)
		return false;

	//Update internal words
	_words = newWords;

	// Create the vertex array.
	vertices = new VertexTextureT[_sentence->vertexCount];
	if(!vertices)
		return false;

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexTextureT) * _sentence->vertexCount));

	//Update internal position
	_posX = newPosX;
	_posY = newPosY;

	// Calculate the X and Y pixel position on the screen to start drawing to.
	_drawX = (float)(((EngineSettings::GetInstance().GetCurrentWindowX() / 2) * -1) + newPosX);
	_drawY = (float)((EngineSettings::GetInstance().GetCurrentWindowY() / 2) - newPosY);


	// Use the font class to build the vertex array from the sentence text and sentence draw location.
	_font->BuildVertexArray((void*)vertices, newWords, _drawX, _drawY);

	// Lock the vertex buffer so it can be written to.
	result = deviceContext->Map(_sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
		return false;

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (VertexTextureT*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexTextureT) * _sentence->vertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(_sentence->vertexBuffer, 0);

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	return true;
}

char* Text::GetWords()
{
	return _words;
}

D3DXVECTOR2 Text::GetPosition()
{
	D3DXVECTOR2 result(_posX, _posY);
	return result;
}

D3DXVECTOR3 Text::GetColor()
{
	D3DXVECTOR3 result(_r, _g, _b);
	return result;
}

GUID Text::GetGuid()
{
	return _guid;
}

bool Text::InitSentence(SentenceT** sentence, int maxLength, ID3D11Device* device)
{
	VertexTextureT* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	// Create a new sentence object.
	*sentence = new SentenceT;
	if(!*sentence)
		return false;

	// Initialize the sentence buffers to null.
	(*sentence)->vertexBuffer = 0;
	(*sentence)->indexBuffer = 0;

	// Set the maximum length of the sentence.
	(*sentence)->maxLength = maxLength;

	// Set the number of vertices in the vertex array.
	(*sentence)->vertexCount = 6 * maxLength;

	// Set the number of indexes in the index array.
	(*sentence)->indexCount = (*sentence)->vertexCount;

	// Create the vertex array.
	vertices = new VertexTextureT[(*sentence)->vertexCount];
	if(!vertices)
		return false;

	// Create the index array.
	indices = new unsigned long[(*sentence)->indexCount];
	if(!indices)
		return false;

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexTextureT) * (*sentence)->vertexCount));

	// Initialize the index array.
	for(i=0; i<(*sentence)->indexCount; i++)
	{
		indices[i] = i;
	}

	// Set up the description of the dynamic vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexTextureT) * (*sentence)->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
	if(FAILED(result))
		return false;

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
	if(FAILED(result))
		return false;

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	// Release the index array as it is no longer needed.
	delete [] indices;
	indices = 0;

	return true;
}