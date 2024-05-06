#include "AIMovementComponent.h"
#include "GameCollisionMngr.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "HobbinComponent.h"
#include "PlayerManager.h"
#include "PointComponent.h"
#include "ScreenManager.h"

dae::AIMovementComponent::AIMovementComponent(dae::GameObject* owner)
	:BaseComponent(owner)
{
	m_pCollision = owner->GetComponent<dae::GameCollisionComponent>();
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

	const auto& pHobbincomp = m_pOwner->GetComponent<dae::HobbinComponent>();

	if (pHobbincomp->ReturnCharacterState() == HobbinComponent::Nobbin)
	{
		if (m_Horizontal)
		{
			bool left = dae::GameCollisionMngr::GetInstance().AIRaycast(m_pOwner->GetRelativePosition(), m_DirLeft, m_pCollision);
			bool right = dae::GameCollisionMngr::GetInstance().AIRaycast(m_pOwner->GetRelativePosition(), m_DirRight, m_pCollision);


			if (right)
			{
				m_CheckRight = true;
			}
			if (left)
			{
				m_CheckLeft = true;
			}


			if (!m_CheckLeft)
			{
				MoveAI(deltaTime, m_DirLeft);
			}
			else if (!m_CheckRight)
			{
				MoveAI(deltaTime, m_DirRight);
			}
		}
		else if (m_Vertical)
		{
			bool up = dae::GameCollisionMngr::GetInstance().AIRaycast(m_pOwner->GetRelativePosition(), m_DirUp, m_pCollision);
			bool down = dae::GameCollisionMngr::GetInstance().AIRaycast(m_pOwner->GetRelativePosition(), m_DirDown, m_pCollision);

			if (up)
			{
				m_CheckUp = true;
			}
			if (down)
			{
				m_CheckDown = true;
			}

			if (!m_CheckUp)
			{
				MoveAI(deltaTime, m_DirUp);
			}
			else if (!m_CheckDown)
			{
				MoveAI(deltaTime, m_DirDown);
			}
		}

		if (m_CheckLeft && m_CheckRight)
		{
			m_Horizontal = false;
			m_Vertical = true;
		}
		if (m_CheckUp && m_CheckDown)
		{
			m_Horizontal = true;
			m_Vertical = false;
		}

		if (m_CheckLeft && m_CheckRight && m_CheckUp && m_CheckDown)
		{
			m_CheckLeft = false;
			m_CheckRight = false;
			m_CheckUp = false;
			m_CheckDown = false;
		}
	}
	else
	{
		GetClosestPlayer();

		const float offset{ 0.3f };

		if(m_Horizontal)
		{

			if (m_pClosestPlayer->GetRelativePosition().y - offset < m_pOwner->GetRelativePosition().y)
			{
				MoveAI(deltaTime, m_DirUp);
			}
			else if(m_pClosestPlayer->GetRelativePosition().y + offset > m_pOwner->GetRelativePosition().y)
			{
				MoveAI(deltaTime, m_DirDown);
			}
			
			if(m_pClosestPlayer->GetRelativePosition().y < m_pOwner->GetRelativePosition().y + offset &&
				m_pClosestPlayer->GetRelativePosition().y > m_pOwner->GetRelativePosition().y - offset)
			{
				m_Vertical = true;
				m_Horizontal = false;
			}
		}
		else if(m_Vertical)
		{

			if (m_pClosestPlayer->GetRelativePosition().x - offset < m_pOwner->GetRelativePosition().x)
			{
				MoveAI(deltaTime, m_DirLeft);
			}
			else if (m_pClosestPlayer->GetRelativePosition().y + offset > m_pOwner->GetRelativePosition().y)
			{
				MoveAI(deltaTime, m_DirRight);
			}

			if (m_pClosestPlayer->GetRelativePosition().x < m_pOwner->GetRelativePosition().x - offset &&
				m_pClosestPlayer->GetRelativePosition().y > m_pOwner->GetRelativePosition().y + offset)
			{
				m_Vertical = false;
				m_Horizontal = true;
			}
		}
	}
}

void dae::AIMovementComponent::MoveAI(float deltaTime, glm::vec2 dir) const
{
	m_pOwner->SetRelativePosition(m_pOwner->GetRelativePosition() + dir * m_Speed * deltaTime);
}

void dae::AIMovementComponent::GetClosestPlayer()
{
	const auto& pPlayers = PlayerManager::GetInstance().GetPlayers();

	if (pPlayers.empty()) return;
	
	float closestdist{ INFINITY };

	for (const auto& player : pPlayers)
	{
		if (player == nullptr) continue;

		const float dist = glm::distance(player->GetRelativePosition(), m_pOwner->GetRelativePosition());
		if (dist < closestdist)
		{
			closestdist = dist;
			m_pClosestPlayer = player.get();
		}
	}
}