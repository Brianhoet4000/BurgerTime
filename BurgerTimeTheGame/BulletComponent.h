#pragma once
#include <glm/vec2.hpp>
#include "BaseComponent.h"

namespace dae
{

	class BulletComponent : public dae::BaseComponent
	{
	public:

		BulletComponent(dae::GameObject* owner);
		virtual ~BulletComponent() override = default;
		BulletComponent(const BulletComponent& other) = delete;
		BulletComponent(BulletComponent&& other) = delete;
		BulletComponent& operator=(const BulletComponent& other) = delete;
		BulletComponent& operator=(BulletComponent&& other) = delete;

		void Update(float deltaTime) override;

	private:
	};

}
