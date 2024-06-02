#pragma once
#include <glm/vec2.hpp>
#include "Scene.h"
#include "GameObject.h"
#include "ShootingDirComponent.h"

namespace dae
{
	class BulletPrefab final
	{
	public:
		BulletPrefab(dae::Scene& scene, glm::vec2 pos, ShootingDirComponent::FaceState Dir);

		~BulletPrefab() = default;
		BulletPrefab(const BulletPrefab& other) = delete;
		BulletPrefab(BulletPrefab&& other) = delete;
		BulletPrefab& operator=(const BulletPrefab& other) = delete;
		BulletPrefab& operator=(BulletPrefab&& other) = delete;

		std::shared_ptr<dae::GameObject> returnGameObject() const { return m_pBullet; }

	private:
		std::shared_ptr<dae::GameObject> m_pBullet;
	};

}
