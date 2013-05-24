#ifndef TEXT_H
#define TEXT_H

#include <D3DX11.h>
#include <D3DX10math.h>

#include "Font.h"
#include "FontShader.h"

class Text
{
private:

	struct Sentence
	{
		ID3D11Buffer* vertexBuffer;
		ID3D11Buffer* indexBuffer;
		int vertexCount, indexCount, maxLength;
		float r, g, b;
	};

	struct VertexTextureType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	Text();
	Text(const Text&);
	~Text();

	bool Init(ID3D11Device*, ID3D11DeviceContext*, HWND, int, int, D3DXMATRIX);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX);

private:
	Font* _font;
	FontShader* _fontShader;
	int _screenHeight, _screenWidth;
	D3DXMATRIX _baseViewMatrix;
	Sentence* _sentence1;
	Sentence* _sentence2;
	Sentence* _sentence3;

	bool InitSentence(Sentence**, int, ID3D11Device*);
	bool UpdateSentence(Sentence*, char*, int, int, float, float, float, ID3D11DeviceContext*);
	void ReleaseSentence(Sentence**);
	bool RenderSentence(ID3D11DeviceContext*, Sentence*, D3DXMATRIX, D3DXMATRIX);
};

#endif