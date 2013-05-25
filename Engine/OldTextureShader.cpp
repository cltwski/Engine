#include "OldTextureShader.h"

OldTextureShader::OldTextureShader()
{
	_vertexShader = NULL;
	_pixelShader = NULL;
	_layout = NULL;
	_matrixBuffer = NULL;
	_samplerState = NULL;
}

OldTextureShader::OldTextureShader(const OldTextureShader& other)
{}

OldTextureShader::~OldTextureShader()
{}

bool OldTextureShader::Init(ID3D11Device* device, HWND hwnd)
{
	bool result;

	//Initialize the vertex and pixel shaders
	result = InitShader(device, hwnd, L"TextureVS.hlsl", L"TexturePS.hlsl");
	if (!result)
		return false;

	return true;
}

void OldTextureShader::Shutdown()
{
	ShutdownShader();
}

bool OldTextureShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	bool result;

	//Set the shader parameters that it will use for rendering
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture);
	if (!result)
		return false;

	//Now render the prepared buffers with the shader
	RenderShader(deviceContext, indexCount);

	return true;
}

bool OldTextureShader::InitShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFileName, WCHAR* psFileName)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;

	//Init pointers to null
	errorMessage = NULL;
	vertexShaderBuffer = NULL;
	pixelShaderBuffer = NULL;

	//Compile the vertex shader code
	result = D3DX10CompileFromFile(vsFileName, NULL, NULL, "TextureVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		//if the shader failed it should have wrriten to the error message
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFileName);
		}
		//if there was nothing in the error message then it simply could not find the shader file itself
		else
		{
			MessageBox(hwnd, vsFileName, L"Missing Vertex Shader File", MB_OK);
		}

		return false;
	}

	//Compile the pixel shader code
	result = D3DX10CompileFromFile(psFileName, NULL, NULL, "TexturePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFileName);
		}
		else
		{
			MessageBox(hwnd, psFileName, L"Missing Pixel Shader File", MB_OK);
		}
		
		return false;
	}

	//Create the vertex and pixel shaders from the buffers
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &_vertexShader);
	if (FAILED(result))
		return false;

	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &_pixelShader);
	if (FAILED(result))
		return false;

	//Now setup the layout of the data that goes into the shader
	//This setup needs to match the VertexType struct in the ModelClass and in the shader
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	//Get count of the elements in the layout
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	//Create the vertex input layout
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &_layout);
	if (FAILED(result))
		return false;

	//Release the vertex shader buffer and pixel shader buffer since they are no longer needed
	vertexShaderBuffer->Release();
	vertexShaderBuffer = NULL;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = NULL;

	//Setup the desciption of the dynamic matrix constant buffer that is in the vertex shader
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	//Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &_matrixBuffer);
	if (FAILED(result))
		return false;

	//Create a texture sampler state description
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//Create the texture sampler satte
	result = device->CreateSamplerState(&samplerDesc, &_samplerState);
	if (FAILED(result))
		return false;

	return true;
}

void OldTextureShader::ShutdownShader()
{
	//Release the sampler state
	if (_samplerState)
	{
		_samplerState->Release();
		_samplerState = NULL;
	}

	//Release the matrix constant buffer
	if (_matrixBuffer)
	{
		_matrixBuffer->Release();
		_matrixBuffer = NULL;
	}

	//Release the layout
	if (_layout)
	{
		_layout->Release();
		_layout = NULL;
	}

	//Release the pixel shader
	if (_pixelShader)
	{
		_pixelShader->Release();
		_pixelShader = NULL;
	}

	//Release the vertex shader
	if (_vertexShader)
	{
		_vertexShader->Release();
		_vertexShader = NULL;
	}
}

void OldTextureShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFileName)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	std::ofstream fout;

	//Get a pointer to the error message text buffer
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	//Get the length of the message
	bufferSize = errorMessage->GetBufferSize();

	//Open a file to write the error message ot
	fout.open("shader-error.txt");

	//Write the error message
	for (i=0; i < bufferSize; ++i)
	{
		fout << compileErrors[i];
	}

	//Close the file
	fout.close();

	//Release the error message
	errorMessage->Release();
	errorMessage = NULL;

	//Pop a message on the screen to notify the user for the text file
	MessageBox(hwnd, L"Error compiling shader. Check shader-error.txt for message.", shaderFileName, MB_OK);
}

bool OldTextureShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	//Transpose the matrices to prepare them for the shader
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

	//Lock the constant buffer so it can be written to
	result = deviceContext->Map(_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
		return false;

	//Get a pointer to the data in the constant buffer
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	//Copy the matrices into the constant buffer
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	//Unlock the constatn buffer
	deviceContext->Unmap(_matrixBuffer, 0);

	//Set the position of the constant buffer in the vertex shader
	bufferNumber = 0;

	//Finally set the constant buffer in the vertex shader with the updated values
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &_matrixBuffer);

	//Set shader texture resource in the pixel shader
	deviceContext->PSSetShaderResources(0, 1, &texture);
}

void OldTextureShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	//Set the vertex input layout
	deviceContext->IASetInputLayout(_layout);

	//Set the vertex and pixel shaders that will be used to render this triangle
	deviceContext->VSSetShader(_vertexShader, NULL, 0);
	deviceContext->PSSetShader(_pixelShader, NULL, 0);

	//Set the sampler state in the pixel shader
	deviceContext->PSSetSamplers(0, 1, &_samplerState);

	//Render the triangle
	deviceContext->DrawIndexed(indexCount, 0, 0);
}

