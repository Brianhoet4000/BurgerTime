#pragma once
#include "BaseComponent.h"
#include "LevelPrefab.h"
#include "Scene.h"

namespace dae
{

	class SpawnTimerComponent : public dae::BaseComponent
	{
	public:
		SpawnTimerComponent(dae::Scene* scene, std::vector<glm::vec2> pos, dae::GameObject* owner);

		virtual ~SpawnTimerComponent() override = default;
		SpawnTimerComponent(const SpawnTimerComponent& other) = delete;
		SpawnTimerComponent(SpawnTimerComponent&& other) = delete;
		SpawnTimerComponent& operator=(const SpawnTimerComponent& other) = delete;
		SpawnTimerComponent& operator=(SpawnTimerComponent&& other) = delete;

		void Update(float deltaTime) override;

	private:
		Scene* m_pScene;
		std::vector<glm::vec2> m_Postions;
		bool m_Switch;
	};

}
