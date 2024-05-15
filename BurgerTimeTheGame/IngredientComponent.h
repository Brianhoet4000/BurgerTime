#pragma once
#include <memory>
#include <vector>
#include "BaseComponent.h"

namespace dae
{

	class IngredientComponent : public dae::BaseComponent
	{
	public:
		IngredientComponent(dae::GameObject* owner);

		virtual ~IngredientComponent() override = default;
		IngredientComponent(const IngredientComponent& other) = delete;
		IngredientComponent(IngredientComponent&& other) = delete;
		IngredientComponent& operator=(const IngredientComponent& other) = delete;
		IngredientComponent& operator=(IngredientComponent&& other) = delete;

		void Update(float) override;
		bool AllArePushedDown() const;

		bool GetIsFalling() const { return m_IsFalling; }

	private:
		int m_speed = 40;
		bool m_allPushedDown;
		bool m_IsFalling;
		bool m_DoOnce;
	};

}
