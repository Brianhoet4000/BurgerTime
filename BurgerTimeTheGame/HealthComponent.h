#pragma once
#include <memory>
#include "BaseComponent.h"
#include "Counter.h"

namespace dae
{
	class HealthComponent : public BaseComponent
	{
	public:

		HealthComponent(dae::GameObject* owner, int startAmount);

		virtual ~HealthComponent() override = default;
		HealthComponent(const HealthComponent& other) = delete;
		HealthComponent(HealthComponent&& other) = delete;
		HealthComponent& operator=(const HealthComponent& other) = delete;
		HealthComponent& operator=(HealthComponent&& other) = delete;

		void DecreaseAmount(int difference) const;
		void IncreaseAmount(int difference) const;
		void SetAmount(int newHealth) const;
		int GetAmount() const { return m_pCounterCp->GetAmount(); }

	private:
		std::unique_ptr<dae::CounterComponent> m_pCounterCp;

	};
}
