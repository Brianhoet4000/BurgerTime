#include "ConditionSingleCoopComponent.h"

#include <iostream>

#include "GameCollisionMngr.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "PlayerManager.h"
#include "ScreenManager.h"
#include "TextureComponent.h"

dae::ConditionSingleCoopComponent::ConditionSingleCoopComponent(dae::GameObject* owner)
	:BaseComponent(owner)
	,m_Finished{false}
{
	
}

void dae::ConditionSingleCoopComponent::Update(float)
{
	for (const auto& player : PlayerManager::GetInstance().GetPlayers())
	{
		if (player->GetComponent<HealthComponent>()->GetAmount() <= -1)
		{
			dae::SceneManager::GetInstance().GetActiveScene()->RemoveAll();
			dae::SceneManager::GetInstance().SetActiveScene("GameOver");
			return;
		}
	}

	
	if(m_Finished && !m_DoOnce && dae::ScreenManager::GetInstance().GetCurrentLevel() != 3)
	{
		dae::ScreenManager::GetInstance().ProceedNextLevel();
		m_DoOnce = true;
	}
}
