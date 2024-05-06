#include "EnemyPrefab.h"
#include "AIMovementComponent.h"
#include "GameCollisionComponent.h"
#include "HobbinComponent.h"
#include "TextureComponent.h"

dae::EnemyPrefab::EnemyPrefab(dae::Scene& scene, glm::vec2 StartPos)
{
	m_pEnemy = std::make_shared<dae::GameObject>("Enemy");
	m_pEnemy->SetRelativePosition(StartPos);

	//Texture
	const auto& pTexture = std::make_shared<dae::TextureComponent>(m_pEnemy.get());
	pTexture->SetTexture("Sprites/Nobbin.png");
	m_pEnemy->AddComponent(pTexture);

	//Collision
	const auto& pCollider = std::make_shared<dae::GameCollisionComponent>(m_pEnemy.get(), StartPos, pTexture->GetSize().x, pTexture->GetSize().y);
	m_pEnemy->AddComponent(pCollider);
	pCollider->SetCollisionRectOffset(0.2f);
	pCollider->SetRenderCollisionBox(true);

	//Hobbin
	const auto& hobbinComponent = std::make_shared<dae::HobbinComponent>(m_pEnemy.get());
	m_pEnemy->AddComponent(hobbinComponent);

	//AIMovement
	const auto& AIMovement = std::make_shared<dae::AIMovementComponent>(m_pEnemy.get());
	m_pEnemy->AddComponent(AIMovement);

	scene.Add(m_pEnemy);
}
