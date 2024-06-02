#include "BulletComponent.h"

#include <memory>
#include <glm/vec2.hpp>

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

	//If in versus mode
	//const auto secondPlayerEnemy = dae::GameCollisionMngr::GetInstance().CheckOverlapWithSecondPlayerVersus(pColliderBullet);
	//if(secondPlayerEnemy != nullptr)
	//{
	//	m_pOwner->MarkTrueForDeleting();
	//	dae::GameCollisionMngr::GetInstance().RemoveBulletBox(pColliderBullet);
	//
	//	//secondPlayerEnemy->GetOwner()->GetComponent<HealthComponent>()->DecreaseAmount(1);
	//	//secondPlayerEnemy->GetOwner()->GetComponent<PointComponent>()->SetAmount(0);
	//
	//	const auto& scene = dae::SceneManager::GetInstance().GetActiveScene();
	//
	//	const auto& points = dae::ScreenManager::GetInstance().GetGameObjectInScene(*scene, "PlayerTwoPoints");
	//	points->GetComponent<TextComponent>()->SetText(std::to_string(secondPlayerEnemy->GetOwner()->GetComponent<PointComponent>()->GetAmount()));
	//
	//	dae::ScreenManager::GetInstance().ProceedNextLevel();
	//	return;
	//}

	//Enemies
	const auto enemy = dae::GameCollisionMngr::GetInstance().CheckOverlapWithEnemiesComponent(pColliderBullet);
	if (enemy != nullptr)
	{
		enemy->GetOwner()->MarkTrueForDeleting();
		dae::GameCollisionMngr::GetInstance().RemoveEnemyBox(enemy->GetOwner()->GetComponent<dae::GameCollisionComponent>());

		m_pOwner->MarkTrueForDeleting();
		dae::GameCollisionMngr::GetInstance().RemoveBulletBox(pColliderBullet);
	}
}
