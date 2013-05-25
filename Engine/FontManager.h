#ifndef FONTMANAGER_H
#define FONTMANAGER_H

#include <unordered_map>

#include "Font.h"

class FontManager
{
public:
	static FontManager& GetInstance();

	void AddFont(Font&, const char*);
	Font* GetFont(const char*);
	void RemoveFont(const char*);

private:
	FontManager();
	FontManager(const FontManager&);
	void operator=(const FontManager&);
	
	std::unordered_map<int, Font> _fonts;

	int Hash(const char*);
};
#endif