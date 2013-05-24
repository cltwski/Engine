#ifndef FONT_H
#define FONT_H

#include <D3D11.h>
#include <D3DX10math.h>
#include <fstream>

#include "Texture.h"

class Font
{
private:
	struct FontType
	{
		float left, right;
		int size;
	};

	struct VertexTextureType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	Font();
	Font(const Font&);
	~Font();

	bool Init(ID3D11Device*, char*, WCHAR*);
	void BuildVertexArray(void*, char*, float, float);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

private:
	FontType* _font;
	Texture* _texture;

	bool LoadFontData(char*);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

};



#endif