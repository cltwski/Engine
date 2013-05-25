#include "Font.h"

Font::Font()
{
	_font = NULL;
	_texture = NULL;
}

Font::Font(const Font&)
{}

Font::~Font()
{}

bool Font::Init(ID3D11Device* device, char* fontFilename, char* textureName)
{
	bool result;

	result = LoadFontData(fontFilename);
	if (!result)
		return false;

	//Load the texture that has the font characters on it
	result = LoadTexture(device, textureName);
	if (!result)
		return false;

	return true;
}

void Font::Shutdown()
{
	ReleaseFontData();
}

bool Font::LoadFontData(char* filename)
{
	std::ifstream fin;
	int i;
	char temp;

	//Create the font spacing buffer
	_font = new FontT[95];
	if (!_font)
		return false;

	//Read in the font size and spacing between chars
	fin.open(filename);
	if (fin.fail())
		return false;

	//Read in the 95 used ascii characters for text
	for (i=0; i < 95; ++i)
	{
		fin.get(temp);
		while(temp != ' ')
			fin.get(temp);
		fin.get(temp);
		while(temp != ' ')
			fin.get(temp);

		fin >> _font[i].left;
		fin >> _font[i].right;
		fin >> _font[i].size;
	}

	fin.close();

	return true;
}

void Font::ReleaseFontData()
{
	if (_font)
	{
		delete[] _font;
		_font = NULL;
	}
}

bool Font::LoadTexture(ID3D11Device* device, char* textureName)
{
	bool result;

	// Create the texture object.
	_texture = TextureManager::GetInstance().GetTexture(textureName);
	if(!_texture)
		return false;

	return true;
}

ID3D11ShaderResourceView* Font::GetTexture()
{
	return _texture->GetTexture();
}


void Font::BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY)
{
	VertexTextureT* vertexPtr;
	int numLetters, index, i, letter;

	//Coerce the input vertices into a VertexTextureTpye structure
	vertexPtr = (VertexTextureT*)vertices;

	//Get the number of letters in the sentence
	numLetters = (int)std::strlen(sentence);

	//Init the index to the vertex array
	index = 0;

	//Draw each letter onto a quad
	for (i=0; i < numLetters; ++i)
	{
		letter = ((int)sentence[i]) -32;

		//If the letter is a space then just move over three pixels
		if (letter == 0)
		{
			drawX += 3.0f;
		}
		else
		{
			// First triangle in quad.
			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f); //Top left
			vertexPtr[index].texture = D3DXVECTOR2(_font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3((drawX + _font[letter].size), (drawY-16), 0.0f); //Bottom right
			vertexPtr[index].texture = D3DXVECTOR2(_font[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX, (drawY-16), 0.0f); //bottom left
			vertexPtr[index].texture = D3DXVECTOR2(_font[letter].left, 1.0f);
			index++;

			// Second triangle in quad.
			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = D3DXVECTOR2(_font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX + _font[letter].size, drawY, 0.0f);  // Top right.
			vertexPtr[index].texture = D3DXVECTOR2(_font[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3((drawX + _font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = D3DXVECTOR2(_font[letter].right, 1.0f);
			index++;

			//Update the x location for drawing by the size of the letter and one pixel
			drawX = drawX + _font[letter].size + 1.0f;
		}
	}
}

