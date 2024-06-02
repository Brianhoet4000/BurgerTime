#include "BulletPrefab.h"

#include "BulletComponent.h"
#include "GameCollisionComponent.h"
#include "TextureComponent.h"

dae::BulletPrefab::BulletPrefab(dae::Scene& scene, glm::vec2 pos, ShootingDirComponent::FaceState Dir)
{
	m_pBullet = std::make_shared<dae::GameObject>("Bullet");

	std::shared_ptr<TextureComponent> pPepperTex = std::make_shared<TextureComponent>(m_pBullet.get());
	pPepperTex->SetTexture("Characters/pepper.png");
	m_pBullet->AddComponent(pPepperTex);

	if (Dir == ShootingDirComponent::Left)
	{
		glm::vec2 newPos = pos;
		newPos.x -= pPepperTex->GetSize().x;
		m_pBullet->SetRelativePosition(newPos);
	}
	else if (Dir == ShootingDirComponent::Right)
	{
		glm::vec2 newPos = pos;
		newPos.x += pPepperTex->GetSize().x;
		m_pBullet->SetRelativePosition(newPos);
	}

	std::shared_ptr<GameCollisionComponent> pPepperColl = std::make_shared<GameCollisionComponent>(m_pBullet.get(), m_pBullet->GetRelativePosition(),
		pPepperTex->GetSize().x , pPepperTex->GetSize().y, false, true);
	pPepperColl->SetDebugColor("red");
	m_pBullet->AddComponent(pPepperColl);

	std::shared_ptr<BulletComponent> pBulletcomp = std::make_shared<BulletComponent>(m_pBullet.get());
	m_pBullet->AddComponent(pBulletcomp);

	scene.Add(m_pBullet);
}
