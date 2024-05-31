#pragma once
#include <memory>
#include <vector>
#include "BaseComponent.h"
#include "GameCollisionComponent.h"

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

		bool GetIsCompleted() const { return m_Completed; }
		void SetIscompleted(bool Completed) { m_Completed = Completed; }

	private:
		int m_speed = 60;
		bool m_allPushedDown;
		bool m_IsFalling;
		bool m_Completed;
		bool m_DoOnce;
		GameCollisionComponent* m_currentFloor;
	};

}
