#pragma once
#include <string>
#include <memory>
#include <vector>
#include "Singleton.h"

namespace dae
{
	class Texture2D;
	class Font;
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void Init(const std::string& data);
		std::shared_ptr<Texture2D> LoadTexture(const std::string& file) const;
		std::shared_ptr<Font> LoadFont(const std::string& file, unsigned int size) const;
		std::vector<int> ParseMapTxt(const std::string& file);
		std::string ReturnDataPath() { return m_dataPath; }
		std::pair<int, int> ReturnGrid() const { return m_Grid; }
	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;
		std::string m_dataPath;
		std::pair<int, int> m_Grid;
	};
}
