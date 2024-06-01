#include <stdexcept>
#include <SDL_ttf.h>
#include "Font.h"

TTF_Font* dae::Font::GetFont() const {
	return m_font;
}

void dae::Font::SetFontSize(unsigned int size) const
{
	TTF_SetFontSize(m_font, size);
}

float dae::Font::GetStringWidth(const std::string& text) const {
    if (!m_font)
    {
        throw std::runtime_error("Font not loaded");
    }

    int width = 0;
    int height = 0;
    if (TTF_SizeText(m_font, text.c_str(), &width, &height) != 0)
    {
        throw std::runtime_error(std::string("Failed to calculate text size: ") + TTF_GetError());
    }

    return static_cast<float>(width);
}

dae::Font::Font(const std::string& fullPath, unsigned int size) : m_font(nullptr), m_size(size)
{
	m_font = TTF_OpenFont(fullPath.c_str(), size);
	if (m_font == nullptr) 
	{
		throw std::runtime_error(std::string("Failed to load font: ") + SDL_GetError());
	}
}

dae::Font::~Font()
{
	TTF_CloseFont(m_font);
}
