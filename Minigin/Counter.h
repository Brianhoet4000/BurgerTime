#pragma once
#include "BaseComponent.h"
#include <string>
#include <iostream>

namespace dae
{

	class CounterComponent : public BaseComponent
	{
	public:
		CounterComponent(GameObject* owner, int startAmount);

		void IncreaseAmount(int difference);
		void DecreaseAmount(int difference);
		void SetAmount(int newAmount);
		int GetAmount() const;

	private:
		int m_Counter{};

	};

}
