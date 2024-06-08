#include "SpawnTimerComponent.h"
#include <iostream>
#include "EnemyPrefab.h"
#include "GameCollisionMngr.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include <cstdlib>  // For rand()
#include <ctime>    // For time()

dae::SpawnTimerComponent::SpawnTimerComponent(dae::Scene* scene, std::vector<glm::vec2> pos ,dae::GameObject* owner)
	:BaseComponent(owner)
	,m_Switch(false)
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	m_pScene = scene;
	m_Postions = pos;
}

void dae::SpawnTimerComponent::Update(float)
{
	if(dae::GameCollisionMngr::GetInstance().GetAllEnemies().size() >= 4)
		return;

	bool hasPickle = false;
	for (const auto& enemy : dae::GameCollisionMngr::GetInstance().GetAllEnemies())
	{
		if (enemy->GetOwner()->GetTag() == "Pickle")
		{
			hasPickle = true;
			break;
		}
	}
	
	if (!hasPickle)
	{
		int randomIndex = std::rand() % 3;
	
		std::shared_ptr<EnemyPrefab> newPickle = 
			std::make_shared<EnemyPrefab>(*m_pScene, m_Postions[randomIndex], "Pickle");
		return;
	}
	else
	{
		int randomIndex = std::rand() % 3;

		if(m_Switch)
		{
			std::shared_ptr<EnemyPrefab> enemy = std::make_shared<EnemyPrefab>(*m_pScene, m_Postions[randomIndex], "HotDog");
			m_Switch = !m_Switch;

		}
		else
		{
			std::shared_ptr<EnemyPrefab> enemy = std::make_shared<EnemyPrefab>(*m_pScene, m_Postions[randomIndex], "Egg");
			m_Switch = !m_Switch;

		}
	}
	
}