#include <stdexcept>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"
#include <iostream>
#include <fstream>
#include <sstream>

void dae::ResourceManager::Init(const std::string& dataPath)
{
	m_dataPath = dataPath;

	if (TTF_Init() != 0)
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

std::shared_ptr<dae::Texture2D> dae::ResourceManager::LoadTexture(const std::string& file) const
{
	const auto fullPath = m_dataPath + file;
	auto texture = IMG_LoadTexture(Renderer::GetInstance().GetSDLRenderer(), fullPath.c_str());
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
	}
	return std::make_shared<Texture2D>(texture);
}

std::shared_ptr<dae::Font> dae::ResourceManager::LoadFont(const std::string& file, unsigned int size) const
{
	return std::make_shared<Font>(m_dataPath + file, size);
}

std::vector<int> dae::ResourceManager::ParseMapTxt(const std::string& file)
{
    std::vector<int> result;
    std::pair<int, int> grid;

    // Open the file for reading
    std::ifstream fileParsing(file);
    if (!fileParsing.is_open())
    {
        // Handle error

        return result;
    }

    // Read each line of the file
    std::string line;
    while (std::getline(fileParsing, line))
    {
        // Parse the line into integers
        std::stringstream ss(line);
        std::string field;
        while (std::getline(ss, field, ','))
        {
            try
            {
                result.push_back(std::stoi(field));
                grid.first++;
            }
            catch (std::invalid_argument&)
            {
                // Handle invalid data
                std::cout << "Invalid data in file\n";
            }
        }
        grid.second++;
    }
    auto newColums = grid.first / grid.second;

    m_Grid = std::make_pair(newColums, grid.second);

    return result;
}
