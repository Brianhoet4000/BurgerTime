#pragma once
#include "BaseComponent.h"

namespace dae
{

	class PlateComponent : public dae::BaseComponent
	{
	public:

		PlateComponent(dae::GameObject* owner);

		virtual ~PlateComponent() override = default;
		PlateComponent(const PlateComponent& other) = delete;
		PlateComponent(PlateComponent&& other) = delete;
		PlateComponent& operator=(const PlateComponent& other) = delete;
		PlateComponent& operator=(PlateComponent&& other) = delete;

		void Update(float) override;
		void IncrementInt() { ++m_AmountOfIngredients; }

	private:
		int m_AmountOfIngredients;

	};

}