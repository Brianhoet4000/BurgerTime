#pragma once
#include "BaseComponent.h"
#include "Scene.h"

namespace dae
{

	class SpawnTimerComponent : public dae::BaseComponent
	{
	public:
		SpawnTimerComponent(dae::Scene* scene, dae::GameObject* owner, float startCountDownNumber, int maxNumberOfEnemies);

		virtual ~SpawnTimerComponent() override = default;
		SpawnTimerComponent(const SpawnTimerComponent& other) = delete;
		SpawnTimerComponent(SpawnTimerComponent&& other) = delete;
		SpawnTimerComponent& operator=(const SpawnTimerComponent& other) = delete;
		SpawnTimerComponent& operator=(SpawnTimerComponent&& other) = delete;

		void Update(float deltaTime) override;
		void SetStartCountingDown(bool CountDownState)
		{
			m_Start = CountDownState;
		}
		bool GetBool() const { return m_Start; }

		int RemainingNumberOfEnemies() const { return m_EnemyNumber; }

	private:
		float m_StartCountDownValue;
		float m_Counter;
		bool m_Start;
		Scene* m_pScene;
		int m_MaxNumberOfEnemies;
		int m_EnemyNumber;
	};

}
