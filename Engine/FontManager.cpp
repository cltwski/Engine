#include "FontManager.h"

FontManager::FontManager()
{
	_device = NULL;
}

FontManager& FontManager::GetInstance()
{
	static FontManager instance;
	return instance;
}

void FontManager::Init(ID3D11Device* device)
{
	_device = device;
}

void FontManager::Shutdown()
{
	for (auto it = _fonts.begin(); it != _fonts.end(); ++it)
	{
		it->second.Shutdown();
	}
	_fonts.clear();
}

bool FontManager::AddFont(char* fontFilename, char* textureName, const char* name)
{
	bool result;

	_fonts.insert(std::make_pair<const char*, Font>(name, Font()));
	result = _fonts[name].Init(_device, fontFilename, textureName);
	if (!result)
		return false;
	/*Font* font = new Font();
	bool result;

	result = font->Init(_device, fontFilename, textureName);
	if (!result)
		return false;

	_fonts.insert(std::make_pair<const char*, Font>(name, *font));
	_fonts[name] = font;*/

	return true;
}

Font* FontManager::GetFont(const char* name)
{
	return &_fonts[name];
}

void FontManager::RemoveFont(const char* name)
{
	_fonts.erase(name);
}
