#pragma once
#include <glm/glm.hpp>
#include "BaseComponent.h"

namespace dae
{
	class Transform final : public BaseComponent
	{
	public:
		Transform(GameObject* owner);
		virtual ~Transform() = default;
		Transform(const Transform& other) = delete;
		Transform(Transform&& other) = delete;
		Transform& operator=(const Transform& other) = delete;
		Transform& operator=(Transform&& other) = delete;

		const glm::vec2& GetPosition() const { return m_position; }
		void SetPosition(float x, float y);

		//void Update(float) override;
	private:
		glm::vec2 m_position = {0,0};
	};
}
