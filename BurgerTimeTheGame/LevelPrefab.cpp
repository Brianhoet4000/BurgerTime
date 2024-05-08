#include "LevelPrefab.h"
#include "ResourceManager.h"
#include "TextureComponent.h"
#include <fstream>
#include "GameCollisionComponent.h"
#include "rapidjson.h"
#pragma warning(disable : 4996)
#include <document.h>

dae::LevelPrefab::LevelPrefab(dae::Scene& scene, const std::string& LevelPath)
{
	m_pLevelObj = std::make_shared<dae::GameObject>();
	m_pLevelObj->SetRelativePosition(43, 50);
	scene.Add(m_pLevelObj);

	//constexpr int width{ 710 };
	//constexpr int height{ 700 };

	const auto& pTexture = std::make_shared<dae::TextureComponent>(m_pLevelObj.get());
	pTexture->SetTexture("../Data/Levels/" + LevelPath + ".png");
	m_pLevelObj->AddComponent(pTexture);



	std::string path = "../Data/Levels/" + LevelPath + ".json";

	LevelParse(scene, LevelPath);
}

void dae::LevelPrefab::LevelParse(dae::Scene& scene, const std::string& LevelPath)
{
	std::string fileName = "../Data/Levels/" + LevelPath + ".json";
	std::ifstream inputFile(fileName);

	// Check if file was opened successfully
	if (!inputFile.is_open())
	{
		// Handle error opening file
		std::cout << "File didn't open\n";
	}

	// Read the entire file into a string
	std::string fileContents((std::istreambuf_iterator<char>(inputFile)),
		std::istreambuf_iterator<char>());

	// Close the input file
	inputFile.close();

	rapidjson::Document document;
	document.Parse(fileContents.c_str());

	for (const auto& playerPos : document["player_spawnpoint"].GetArray())
	{
		float x = playerPos[0].GetFloat();
		float y = playerPos[1].GetFloat();
		m_SpawnPositions.push_back(glm::vec2{ x,y });
	}

	for (const auto& stair : document["stairs"].GetArray())
	{
		float x = stair[0].GetFloat();
		float y = stair[1].GetFloat();
		float z = stair[2].GetFloat();

		std::shared_ptr<dae::GameObject> ladder = std::make_shared<dae::GameObject>("Stair");
		std::shared_ptr<dae::GameCollisionComponent> LadderColl =
			std::make_shared<dae::GameCollisionComponent>(ladder.get(), glm::vec2{ x + 43.f, y + 43.f },
				48.f, z + 8.f, false, true);
		ladder->AddComponent(LadderColl);
		scene.Add(ladder);
	}

	for (const auto& stair : document["floors"].GetArray())
	{
		float x = stair[0].GetFloat();
		float y = stair[1].GetFloat();
		float z = stair[2].GetFloat();

		std::shared_ptr<dae::GameObject> floor = std::make_shared<dae::GameObject>("Floor");
		std::shared_ptr<dae::GameCollisionComponent> FloorColl =
			std::make_shared<dae::GameCollisionComponent>(floor.get(), glm::vec2{ x + 43.f, y + 42.f },
				z,  10.f, false, true);
		FloorColl->SetDebugColor("orange");
		floor->AddComponent(FloorColl);
		scene.Add(floor);
	}

}
