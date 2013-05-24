#ifndef TEXTURE_H
#define TEXTURE_H

#include <D3D11.h>
#include <D3DX11tex.h>

class Texture
{
public:
	Texture();
	Texture(const Texture&);
	~Texture();

	bool Init(ID3D11Device*, WCHAR*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

private:
	ID3D11ShaderResourceView* _texture;
};

#endif