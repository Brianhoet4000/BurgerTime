#include "EnemyStates.h"

EnemyMovement::EnemyMovement(dae::GameObject* owner, glm::vec2 startPos)
	:m_pOwner(owner)
{
	m_LeftOfStartPos = glm::vec2{ startPos.x - 25, startPos.y };
	m_RightOfStartPos = glm::vec2{ startPos.x + 25, startPos.y };

	m_Dir = m_DirRight;
}

void EnemyMovement::OnEnter()
{
	m_update = true;
}

void EnemyMovement::Update(float deltaTime)
{
	if (!m_update) return;
	
	m_pOwner->SetRelativePosition(m_pOwner->GetRelativePosition() + m_Dir * 40.f * deltaTime);
	
	
	if (m_pOwner->GetRelativePosition().x < m_LeftOfStartPos.x)
	{
		m_Dir = m_DirRight;
	}
	else if(m_pOwner->GetRelativePosition().x > m_RightOfStartPos.x)
	{
		m_Dir = m_DirLeft;
	}
	
}

void EnemyMovement::OnExit()
{
	m_update = false;
}

EnemyStunned::EnemyStunned(dae::GameObject* owner)
	:m_pOwner(owner)
{
	m_pStunTimer = std::make_unique<dae::BulletTimerComponent>(owner);
}

void EnemyStunned::OnEnter()
{
}

void EnemyStunned::Update(float)
{
	if (!m_update) return;
	if (m_pStunTimer->ReturnHasShot()) return;

	m_pStunTimer->SetHasShot(true);
	std::cout << "Stunned\n";

	if (m_pStunTimer->ReturnCompleted())
	{
		m_update = false;
		std::cout << "Completed\n";
	}
}

void EnemyStunned::OnExit()
{
}
