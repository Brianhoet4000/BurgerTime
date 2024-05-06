#pragma once
#include <string>
#include <memory>
#include "Transform.h"
#include "BaseComponent.h"

namespace dae
{
	class RotationComponent final : public BaseComponent
	{
	public:
		virtual void Update(float deltaTime) override;

		RotationComponent(GameObject* owner, float offset, float rotationSpeed);
		virtual ~RotationComponent() = default;
		RotationComponent(const RotationComponent& other) = delete;
		RotationComponent(RotationComponent&& other) = delete;
		RotationComponent& operator=(const RotationComponent& other) = delete;
		RotationComponent& operator=(RotationComponent&& other) = delete;
	private:
		float m_Radius{};
		float m_RotationSpeed{};
		float m_Angle{};
		glm::vec2 m_OffsetOfTheParent{};
	};
}