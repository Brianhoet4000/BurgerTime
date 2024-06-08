#pragma once
#include "BaseComponent.h"
#include "Counter.h"

namespace dae
{

	class BulletTimerComponent : public dae::BaseComponent
	{
	public:

		BulletTimerComponent(dae::GameObject* owner, const float timer);
		virtual ~BulletTimerComponent() override = default;
		BulletTimerComponent(const BulletTimerComponent& other) = delete;
		BulletTimerComponent(BulletTimerComponent&& other) = delete;
		BulletTimerComponent& operator=(const BulletTimerComponent& other) = delete;
		BulletTimerComponent& operator=(BulletTimerComponent&& other) = delete;

		void Update(float deltaTime) override;
		bool ReturnHasShot() const { return m_HasShot; }
		void ResetComplete() { m_Completed = false; }
		bool ReturnCompleted() const { return m_Completed; }
		void SetHasShot(bool hasShot) { m_HasShot = hasShot; }

	private:
		float m_StartValue;
		float m_CurrentTime;
		bool m_HasShot = false;
		bool m_Completed = false;
		CounterComponent* m_pCounter;
	};

}
