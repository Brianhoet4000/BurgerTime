#include "BulletComponent.h"

#include <memory>
#include <thread>
#include <glm/vec2.hpp>

#include "AIMovementComponent.h"
#include "GameCollisionMngr.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "PointComponent.h"
#include "ScreenManager.h"
#include "TextureComponent.h"


dae::BulletComponent::BulletComponent(dae::GameObject* owner)
	:BaseComponent(owner)
{
	
}

void dae::BulletComponent::Update(float)
{
	const auto pColliderBullet = m_pOwner->GetComponent<dae::GameCollisionComponent>();

	if(m_pOwner->GetComponent<BulletTimerComponent>()->ReturnCompleted())
		m_pOwner->MarkTrueForDeleting();

	//If in versus mode
	const auto secondPlayerEnemy = dae::GameCollisionMngr::GetInstance().CheckOverlapWithSecondPlayerVersus(pColliderBullet);
	if(secondPlayerEnemy != nullptr)
	{
		m_pOwner->MarkTrueForDeleting();
		dae::GameCollisionMngr::GetInstance().RemoveBulletBox(pColliderBullet);

		secondPlayerEnemy->GetOwner()->GetComponent<BulletTimerComponent>()->SetHasShot(true);

		return;
	}

	//Enemies
	const auto enemy = dae::GameCollisionMngr::GetInstance().CheckOverlapWithEnemiesComponent(pColliderBullet);
	if (enemy != nullptr)
	{
		if (m_pOwner->GetComponent<BulletTimerComponent>()->ReturnCompleted())
		{
			m_pOwner->MarkTrueForDeleting();
			dae::GameCollisionMngr::GetInstance().RemoveBulletBox(pColliderBullet);
		}

		enemy->GetOwner()->GetComponent<AIMovementComponent>()->Stunned();
	}
}
