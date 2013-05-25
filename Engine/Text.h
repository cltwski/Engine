#ifndef TEXT_H
#define TEXT_H

#include <D3DX11.h>
#include <D3DX10math.h>

#include "Font.h"
#include "OldFontShader.h"

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

	bool Init(ID3D11Device*, ID3D11DeviceContext*, HWND, int, int, D3DXMATRIX, char*, int=0, int=0, float=1.0f, float=1.0f, float=1.0f);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX);
	bool UpdateSentence(char*, ID3D11DeviceContext*);
	bool UpdateSentence(char*, float, float, float, ID3D11DeviceContext*);
	bool UpdateSentence(char*, int, int, float, float, float, ID3D11DeviceContext*);
	char* GetWords();
	GUID GetGuid();

protected:
	Font* _font;
	OldFontShader* _fontShader;
	int _screenHeight, _screenWidth;
	float _drawX, _drawY;
	D3DXMATRIX _baseViewMatrix;
	Sentence* _sentence;
	char* _words;
	GUID _guid;

	bool InitSentence(Sentence**, int, ID3D11Device*);
	void ReleaseSentence(Sentence**);
	bool RenderSentence(ID3D11DeviceContext*, Sentence*, D3DXMATRIX, D3DXMATRIX);
};

#endif