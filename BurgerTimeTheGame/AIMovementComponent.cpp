#include "AIMovementComponent.h"
#include "GameCollisionMngr.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "PlayerManager.h"
#include "PointComponent.h"
#include "ScreenManager.h"

dae::AIMovementComponent::AIMovementComponent(dae::GameObject* owner, glm::vec2 startPos)
	:BaseComponent(owner)
{
	m_pCollision = owner->GetComponent<dae::GameCollisionComponent>();
	m_pStunTimer = std::make_unique<dae::BulletTimerComponent>(owner);

	m_pEnemyMovement = std::make_unique<EnemyMovement>(m_pOwner, startPos);
	m_pEnemyStunned = std::make_unique<EnemyStunned>(m_pOwner, m_pStunTimer.get());
	
	m_pEnemyMovement->OnEnter();
}

void dae::AIMovementComponent::Update(float deltaTime)
{
	if (m_pOwner->ReturnDeleting()) return;

	const auto& pPlayerCollision = dae::GameCollisionMngr::GetInstance().CheckOverlapWithPlayers(m_pOwner->GetComponent<dae::GameCollisionComponent>());
	if (pPlayerCollision != nullptr)
	{
		dae::ScreenManager::GetInstance().PlayerKilledResetLevelAndStats(pPlayerCollision);
		return;
	}

	if(m_pEnemyStunned->DoneStun() || !m_pStunTimer->ReturnHasShot())
	{
		m_pEnemyStunned->OnExit();
		m_pStunTimer->ResetComplete();
		m_pEnemyMovement->OnEnter();
	}

	

	m_pEnemyMovement->Update(deltaTime);
	m_pEnemyStunned->Update(deltaTime);
}
