#include "EnemyPrefab.h"
#include "AIMovementComponent.h"
#include "GameCollisionComponent.h"
#include "RandomSpawn.h"
#include "TextureComponent.h"

dae::EnemyPrefab::EnemyPrefab(dae::Scene& scene, glm::vec2 StartPos, std::string typeOfenemy)
{
	m_pEnemy = std::make_shared<dae::GameObject>(typeOfenemy);

	//Texture
	const auto& pTexture = std::make_shared<dae::TextureComponent>(m_pEnemy.get());
	pTexture->SetTexture("Enemies/" + typeOfenemy + ".png");
	m_pEnemy->AddComponent(pTexture);

	m_pEnemy->SetRelativePosition(StartPos.x, StartPos.y - pTexture->GetSize().y);

	const auto& pFootCollider = std::make_shared<dae::GameCollisionComponent>(m_pEnemy.get(), m_pEnemy->GetRelativePosition(),
		pTexture->GetSize().x, 6.f, true, false, false);
	m_pEnemy->AddComponent(pFootCollider);
	pFootCollider->SetCollisionRectOffset(3.f, pTexture->GetSize().y - 5.f, true);
	//pFootCollider->SetDebugColor("green");
	//pFootCollider->SetRenderCollisionBox(true);

	//AIMovement
	const auto& AIMovement = std::make_shared<dae::AIMovementComponent>(m_pEnemy.get(), StartPos);
	m_pEnemy->AddComponent(AIMovement);

	scene.Add(m_pEnemy);
}
