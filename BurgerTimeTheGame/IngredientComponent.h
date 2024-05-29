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
		void SetIsFalling(bool Falling) { m_IsFalling = Falling; }

	private:
		int m_speed = 60;
		bool m_allPushedDown;
		bool m_IsFalling;
		bool m_OverlappedFloor;
	};

}
