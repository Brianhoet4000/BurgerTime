#pragma once
#include <vector>
#include <glm/vec2.hpp>
#include "GameObject.h"
#include "Scene.h"

#pragma warning(disable : 4996)
#include <document.h>

namespace dae
{
	class LevelPrefab final
	{
	public:
		LevelPrefab(dae::Scene& scene,const std::string& LevelPath);

		~LevelPrefab() = default;
		LevelPrefab(const LevelPrefab& other) = delete;
		LevelPrefab(LevelPrefab&& other) = delete;
		LevelPrefab& operator=(const LevelPrefab& other) = delete;
		LevelPrefab& operator=(LevelPrefab&& other) = delete;

		void LevelParse(dae::Scene& scene, const std::string& LevelPath);

		std::vector<glm::vec2> GetSpawnPosition() const { return m_SpawnPositions; }
		glm::vec2 GetEnemySpawnPosition() const { return m_EnemySpawnPositions; }

		std::shared_ptr<GameObject> returnLevelObj() { return m_pLevelObj; }

	private:
		std::vector<glm::vec2> m_SpawnPositions{};
		glm::vec2 m_EnemySpawnPositions{};

		std::shared_ptr<GameObject> m_pLevelObj;

		void ParseIngredient(Scene& scene, const rapidjson::Document& document, const std::string& part);
	};

}