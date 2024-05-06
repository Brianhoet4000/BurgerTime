#pragma once
#include <glm/vec2.hpp>
#include "BaseComponent.h"

namespace dae
{

	class BulletComponent : public dae::BaseComponent
	{
	public:

		BulletComponent(dae::GameObject* owner, glm::vec2 vel, int amountOfBounces);
		virtual ~BulletComponent() override = default;
		BulletComponent(const BulletComponent& other) = delete;
		BulletComponent(BulletComponent&& other) = delete;
		BulletComponent& operator=(const BulletComponent& other) = delete;
		BulletComponent& operator=(BulletComponent&& other) = delete;

		void Update(float deltaTime) override;

	private:
		glm::vec2 m_vel;
		int m_Bounce;
		int m_AmountOfBounce;
		const float m_Speed;
	};

}
