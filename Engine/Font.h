#ifndef FONT_H
#define FONT_H

#include <D3D11.h>
#include <D3DX10math.h>
#include <fstream>

#include "TextureManager.h"

class Font
{
protected:
	struct FontT
	{
		float left, right;
		int size;
	};

	struct VertexTextureT
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	Font();
	Font(const Font&);
	~Font();

	bool Init(ID3D11Device*, char*, char*);
	void BuildVertexArray(void*, char*, float, float);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

protected:
	FontT* _font;
	Texture* _texture;

	bool LoadFontData(char*);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device*, char*);

};



#endif