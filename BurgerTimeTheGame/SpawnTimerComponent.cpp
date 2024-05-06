#include "SpawnTimerComponent.h"
#include <iostream>

#include "EnemyPrefab.h"
#include "GameObject.h"
#include "TextureComponent.h"

dae::SpawnTimerComponent::SpawnTimerComponent(dae::Scene* scene, dae::GameObject* owner,float startCountDownNumber, int maxNumberOfEnemies)
	:BaseComponent(owner)
	,m_StartCountDownValue{startCountDownNumber}
	,m_MaxNumberOfEnemies{maxNumberOfEnemies}
	,m_EnemyNumber{maxNumberOfEnemies}
{
	m_Start = false;
	m_Counter = m_StartCountDownValue;
	m_pScene = scene;
}

void dae::SpawnTimerComponent::Update(float deltaTime)
{
	if (!m_Start)
	{
		auto enemy = std::make_shared<dae::EnemyPrefab>(*m_pScene, m_pOwner->GetRelativePosition());
		m_pScene->Add(enemy->returnGameObject());
		--m_EnemyNumber;
		m_Start = true;

		if(m_EnemyNumber == 0)
		{
			auto pTexture = m_pOwner->GetComponent<dae::TextureComponent>();
			pTexture->SetMustRender(true);
		}
	}


	if (m_Start)
	{
		m_Counter -= 1 * deltaTime;

		if (m_Counter <= 0)
		{
			if (m_EnemyNumber != 0)
			{
				m_Start = false;
				m_Counter = m_StartCountDownValue;
			}
		}
	}
}