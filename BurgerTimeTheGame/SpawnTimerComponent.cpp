#include "SpawnTimerComponent.h"
#include <iostream>

#include "EnemyPrefab.h"
#include "GameCollisionMngr.h"
#include "GameObject.h"
#include "TextureComponent.h"

dae::SpawnTimerComponent::SpawnTimerComponent(dae::Scene* scene, dae::GameObject* owner)
	:BaseComponent(owner)
{
	m_pScene = scene;
}

void dae::SpawnTimerComponent::Update(float)
{
	if(dae::GameCollisionMngr::GetInstance().GetAllEnemies().size() >= 4)
		return;

	
	// Step 2: Check if there is already a "Pickle" in the scene
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
		//auto newPickle = std::make_shared<EnemyPrefab>(*m_pScene, , "Pickle");
	}
	
}