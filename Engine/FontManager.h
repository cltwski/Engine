#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <D3D11.h>
#include <unordered_map>

#include "Font.h"

class FontManager
{
public:
	static FontManager& GetInstance();
	void Init(ID3D11Device*);
	void Shutdown();

	bool AddFont(char*, char*, const char*);
	Font* GetFont(const char*);
	void RemoveFont(const char*);

private:
	FontManager();
	FontManager(const FontManager&);
	void operator=(const FontManager&);
	
	ID3D11Device* _device;
	std::unordered_map<const char*, Font> _fonts;
};
#endif