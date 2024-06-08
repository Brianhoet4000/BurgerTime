#pragma once
#include <vector>
#include <glm/vec2.hpp>

#include "BaseComponent.h"
#include "LevelPrefab.h"

namespace dae
{
	class SpawnComponent : public dae::BaseComponent
	{
	public:

		SpawnComponent(dae::GameObject* owner, glm::vec2 pos);

		virtual ~SpawnComponent() override = default;
		SpawnComponent(const SpawnComponent& other) = delete;
		SpawnComponent(SpawnComponent&& other) = delete;
		SpawnComponent& operator=(const SpawnComponent& other) = delete;
		SpawnComponent& operator=(SpawnComponent&& other) = delete;

		glm::vec2 GetPosition() const;

	private:
		glm::vec2 m_SpawnPos;
	};

}