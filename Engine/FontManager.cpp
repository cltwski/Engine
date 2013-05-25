#include "FontManager.h"

FontManager::FontManager()
{}

FontManager& FontManager::GetInstance()
{
	static FontManager instance;
	return instance;
}

void FontManager::AddFont(Font& font, const char* name)
{
	int id = Hash(name);
	_fonts.insert(std::make_pair<int, Font>(id, font));
}

Font* FontManager::GetFont(const char* name)
{
	int id = Hash(name);
	return &_fonts[id];
}

void FontManager::RemoveFont(const char* name)
{
	int id = Hash(name);
	_fonts.erase(id);
}

int FontManager::Hash(const char* str)
{
	//dbj2
	unsigned long hash = 5381;
	int c;

	while (c = *str++)
	{
		hash = ((hash << 5) + hash) + c;
	}
	return hash;
}