#include "Text.h"

Text::Text()
{
	_font = NULL;
	_fontShader = NULL;
	_sentence1 = NULL;
	_sentence2 = NULL;
	_sentence3 = NULL;
}

Text::Text(const Text& other)
{}

Text::~Text()
{}

bool Text::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight, D3DXMATRIX baseViewMatrix)
{
	bool result;

	//Store the screen width and heigh
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;

	_baseViewMatrix = baseViewMatrix;

	//Create the font and font shader objects
	_font = new Font();
	if (!_font)
		return false;

	result = _font->Init(device, "Data/Fonts/fontdata.txt", L"Data/Fonts/font.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not init the font object", L"Error", MB_OK);
		return false;
	}

	_fontShader = new FontShader();
	if (!_fontShader)
		return false;

	result = _fontShader->Init(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not init the font shader object", L"Error", MB_OK);
		return false;
	}

	//Init the first sentence
	result = InitSentence(&_sentence1, 16, device);
	if (!result)
		return false;

	//Update the sentence vertex buffer with the new string info
	result = UpdateSentence(_sentence1, "Test1", 420, 100, 1.0f, 0.0f, 1.0f, deviceContext);
	if (!result)
		return false;

	//and now the second sentence
	result = InitSentence(&_sentence2, 16, device);
	if (!result)
		return false;

	//Update the sentence vertex buffer with the new string info
	result = UpdateSentence(_sentence2, "Test2", 200, 200, 0.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
		return false;

	result = InitSentence(&_sentence3, 16, device);
	if (!result)
		return false;

	result = UpdateSentence(_sentence3, "Test 3", 100, 600, 0.0f, 0.0f, 1.0f, deviceContext);
	if (!result)
		return false;

	return true;
}

void Text::Shutdown()
{
	ReleaseSentence(&_sentence1);

	ReleaseSentence(&_sentence2);

	ReleaseSentence(&_sentence3);

	if (_fontShader)
	{
		_fontShader->Shutdown();
		delete _fontShader;
		_fontShader = NULL;
	}

	if (_font)
	{
		_font->Shutdown();
		delete _font;
		_font = NULL;
	}
}

bool Text::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix)
{
	bool result;

	// Draw the first sentence.
	result = RenderSentence(deviceContext, _sentence1, worldMatrix, orthoMatrix);
	if(!result)
		return false;

	// Draw the second sentence.
	result = RenderSentence(deviceContext, _sentence2, worldMatrix, orthoMatrix);
	if(!result)
		return false;

	result = RenderSentence(deviceContext, _sentence3, worldMatrix, orthoMatrix);
	if (!result)
		return false;

	return true;
}

bool Text::InitSentence(Sentence** sentence, int maxLength, ID3D11Device* device)
{
	VertexTextureType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	// Create a new sentence object.
	*sentence = new Sentence;
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
	vertices = new VertexTextureType[(*sentence)->vertexCount];
	if(!vertices)
		return false;

	// Create the index array.
	indices = new unsigned long[(*sentence)->indexCount];
	if(!indices)
		return false;

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexTextureType) * (*sentence)->vertexCount));

	// Initialize the index array.
	for(i=0; i<(*sentence)->indexCount; i++)
	{
		indices[i] = i;
	}

	// Set up the description of the dynamic vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexTextureType) * (*sentence)->vertexCount;
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

bool Text::UpdateSentence(Sentence* sentence, char* text, int posX, int posY, float red, float green, float blue,
			       ID3D11DeviceContext* deviceContext)
{
	int numLetters;
	VertexTextureType* vertices;
	float drawX, drawY;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexTextureType* verticesPtr;

	// Store the color of the sentence.
	sentence->r = red;
	sentence->g = green;
	sentence->b = blue;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(text);

	// Check for possible buffer overflow.
	if(numLetters > sentence->maxLength)
		return false;

	// Create the vertex array.
	vertices = new VertexTextureType[sentence->vertexCount];
	if(!vertices)
		return false;

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexTextureType) * sentence->vertexCount));

	// Calculate the X and Y pixel position on the screen to start drawing to.
	drawX = (float)(((_screenWidth / 2) * -1) + posX);
	drawY = (float)((_screenHeight / 2) - posY);

	// Use the font class to build the vertex array from the sentence text and sentence draw location.
	_font->BuildVertexArray((void*)vertices, text, drawX, drawY);

	// Lock the vertex buffer so it can be written to.
	result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
		return false;

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (VertexTextureType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexTextureType) * sentence->vertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(sentence->vertexBuffer, 0);

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	return true;
}

void Text::ReleaseSentence(Sentence** sentence)
{
	if(*sentence)
	{
		// Release the sentence vertex buffer.
		if((*sentence)->vertexBuffer)
		{
			(*sentence)->vertexBuffer->Release();
			(*sentence)->vertexBuffer = NULL;
		}

		// Release the sentence index buffer.
		if((*sentence)->indexBuffer)
		{
			(*sentence)->indexBuffer->Release();
			(*sentence)->indexBuffer = NULL;
		}

		// Release the sentence.
		delete *sentence;
		*sentence = NULL;
	}

	return;
}

bool Text::RenderSentence(ID3D11DeviceContext* deviceContext, Sentence* sentence, D3DXMATRIX worldMatrix, 
			       D3DXMATRIX orthoMatrix)
{
	unsigned int stride, offset;
	D3DXVECTOR4 pixelColor;
	bool result;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexTextureType); 
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create a pixel color vector with the input sentence color.
	pixelColor = D3DXVECTOR4(sentence->r, sentence->g, sentence->b, 1.0f);

	// Render the text using the font shader.
	result = _fontShader->Render(deviceContext, sentence->indexCount, worldMatrix, _baseViewMatrix, orthoMatrix, _font->GetTexture(), 
				      pixelColor);
	if(!result)
	{
		false;
	}

	return true;
}