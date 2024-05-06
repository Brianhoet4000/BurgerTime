#pragma once
#include <memory>

#include "BaseComponent.h"
#include "Counter.h"

namespace dae
{

	class PointComponent : public BaseComponent
	{
	public:

		PointComponent(dae::GameObject* owner, int startamount);

		virtual ~PointComponent() override = default;
		PointComponent(const PointComponent& other) = delete;
		PointComponent(PointComponent&& other) = delete;
		PointComponent& operator=(const PointComponent& other) = delete;
		PointComponent& operator=(PointComponent&& other) = delete;

		void DecreaseAmount(int difference) const;
		void IncreaseAmount(int difference) const;
		void SetAmount(int newInt) const;
		int GetAmount() const { return m_pCounterCp->GetAmount(); }

	private:
		std::unique_ptr<dae::CounterComponent> m_pCounterCp;

	};

}
