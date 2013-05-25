#pragma once

#include <D3DX11.h>
#include <D3DX10math.h>

#include "FontManager.h"
#include "ShaderManager.h"
#include "EngineSettings.h"

class Text
{
protected:
	struct SentenceT
	{
		ID3D11Buffer* vertexBuffer;
		ID3D11Buffer* indexBuffer;
		int vertexCount, indexCount, maxLength;
		float red, green, blue;
	};

	struct VertexTextureT
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	Text();
	Text(const Text&);
	~Text();

	bool Init(ID3D11Device*, ID3D11DeviceContext*, HWND, char*, char*, D3DXMATRIX, char*, int, int, float, float, float);
	bool Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX);
	void Shutdown();

	bool UpdateWords(char*, ID3D11DeviceContext*);
	bool UpdateColor(float, float, float, ID3D11DeviceContext*);
	bool UpdatePosition(int, int, ID3D11DeviceContext*);
	bool UpdateSentence(char*, int, int, float, float, float, ID3D11DeviceContext*);

	char* GetWords();
	D3DXVECTOR2 GetPosition();
	D3DXVECTOR3 GetColor();
	GUID GetGuid();

protected:
	Font* _font;
	Shader* _shader;
	float _drawX, _drawY;
	float _posX, _posY;
	float _r, _g, _b;
	D3DXMATRIX _baseViewMatrix;
	SentenceT* _sentence;
	char* _words;
	GUID _guid;

	bool InitSentence(SentenceT**, int, ID3D11Device*);
};