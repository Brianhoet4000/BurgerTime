#include "ConditionVersusComponent.h"

#include "GameCollisionMngr.h"
#include "HealthComponent.h"
#include "PlayerManager.h"
#include "SceneManager.h"
#include "ScreenManager.h"

dae::ConditionVersusComponent::ConditionVersusComponent(dae::GameObject* owner)
	:BaseComponent(owner)
	,m_Finished{ false }
{
}

void dae::ConditionVersusComponent::Update(float)
{
	for (const auto& player : PlayerManager::GetInstance().GetPlayers())
	{
		if (player->GetComponent<dae::HealthComponent>()->GetAmount() <= -1)
		{
			dae::SceneManager::GetInstance().GetActiveScene()->RemoveAll();
			dae::SceneManager::GetInstance().SetActiveScene("GameOver");
			return;
		}
	}

	if (m_Finished && !m_DoOnce && dae::ScreenManager::GetInstance().GetCurrentLevel() != 3)
	{
		dae::ScreenManager::GetInstance().ProceedNextLevel();
		m_DoOnce = true;
	}
}
