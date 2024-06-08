#include "LevelPrefab.h"
#include "ResourceManager.h"
#include "TextureComponent.h"
#include <fstream>
#include "GameCollisionComponent.h"
#include "Ingredient.h"
#include "IngredientPart.h"
#include "PlateComponent.h"
#include "rapidjson.h"
#pragma warning(disable : 4996)
#include <document.h>

#include "EnemyPrefab.h"


dae::LevelPrefab::LevelPrefab(dae::Scene& scene, const std::string& LevelPath)
{
	m_pLevelObj = std::make_shared<dae::GameObject>(LevelPath);
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

	for (const auto& floors : document["floors"].GetArray())
	{
		float x = floors[0].GetFloat();
		float y = floors[1].GetFloat();
		float z = floors[2].GetFloat();

		std::shared_ptr<dae::GameObject> floor = std::make_shared<dae::GameObject>("Floor");
		std::shared_ptr<dae::GameCollisionComponent> FloorColl =
			std::make_shared<dae::GameCollisionComponent>(floor.get(), glm::vec2{ x + 43.f, y + 42.f },
				z,  10.f, false, true);
		FloorColl->SetDebugColor("red");
		floor->AddComponent(FloorColl);
		scene.Add(floor);
	}

	for (const auto& plates : document["plates"].GetArray())
	{
		float x = plates[0].GetFloat();
		float y = plates[1].GetFloat();

		std::shared_ptr<dae::GameObject> plate = std::make_shared<dae::GameObject>("Plate");
		std::shared_ptr<dae::GameCollisionComponent> PlateColl =
			std::make_shared<dae::GameCollisionComponent>(plate.get(), glm::vec2{ x + 36.f, y - 30.f },
				104.f, 84.f, false, false);
		std::shared_ptr<dae::PlateComponent> PlateComp = std::make_shared<PlateComponent>(plate.get());
		PlateColl->SetDebugColor("red");
		plate->AddComponent(PlateColl);
		plate->AddComponent(PlateComp);
		scene.Add(plate);
	}

	//for (const auto& Enemies : document["enemy_spawnpoint"].GetArray())
	//{
	//	float x = Enemies[0].GetFloat();
	//	float y = Enemies[1].GetFloat();
	//	std::string TypeEnemy = Enemies[2][0].GetString();
	//	m_EnemySpawnPositions.emplace_back(glm::vec2{ x,y });
	//
	//	std::shared_ptr<EnemyPrefab> enemy = std::make_shared<EnemyPrefab>(scene, glm::vec2{ x,y }, TypeEnemy);
	//}

	// BUN_TOP
	ParseIngredient(scene, document, "bun_top");
	// LETTUCE
	ParseIngredient(scene, document, "lettuce");
	// TOMATO
	ParseIngredient(scene, document, "tomato");
	// CHEESE
	ParseIngredient(scene, document, "cheese");
	// PATTY
	ParseIngredient(scene, document, "patty");
	// BUN_BOTTOM
	ParseIngredient(scene, document, "bun_bottom");
}

void dae::LevelPrefab::ParseIngredient(Scene& scene, const rapidjson::Document& document, const std::string& part)
{
	const rapidjson::Value& bunBottomValue = document[part.c_str()];
	const std::string bunBottomTexturePath = bunBottomValue["texturePath"].GetString();
	for (const auto& bunBottom : bunBottomValue["position"].GetArray())
	std::unique_ptr<Ingredient> newPatty = std::make_unique<Ingredient>(bunBottomTexturePath, scene, glm::vec2{ bunBottom[0].GetDouble() + 43.f,bunBottom[1].GetDouble() + 32.f });
}
