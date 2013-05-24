#include "Texture.h"

Texture::Texture()
{
	_texture = NULL;
}

Texture::Texture(const Texture& other)
{}

Texture::~Texture()
{}

bool Texture::Init(ID3D11Device* device, WCHAR* filename)
{
	HRESULT result;

	//Load the texture in
	result = D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &_texture, NULL);
	if (FAILED(result))
		return false;

	return true;
}

void Texture::Shutdown()
{
	//Release the texture resource
	if (_texture)
	{
		_texture->Release();
		_texture = NULL;
	}
}

ID3D11ShaderResourceView* Texture::GetTexture()
{
	return _texture;
}