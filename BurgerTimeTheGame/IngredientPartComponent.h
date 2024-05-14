#pragma once
#include <iostream>

#include "BaseComponent.h"


namespace dae
{

	class IngredientPartComponent : public dae::BaseComponent
	{
	public:
		IngredientPartComponent(dae::GameObject* owner);

		virtual ~IngredientPartComponent() override = default;
		IngredientPartComponent(const IngredientPartComponent& other) = delete;
		IngredientPartComponent(IngredientPartComponent&& other) = delete;
		IngredientPartComponent& operator=(const IngredientPartComponent& other) = delete;
		IngredientPartComponent& operator=(IngredientPartComponent&& other) = delete;

		void Update(float deltaTime) override;

		bool GetPushedDown() const { return m_PushedDown; }
		void SetPushedDown(bool pushedDown)
		{
			m_PushedDown = pushedDown;
		}

		bool GetCollided() const { return m_collided; }
		void SetCollided(bool collided)
		{
			m_collided = collided;
		}

	private:
		bool m_PushedDown;
		bool m_collided;
	};

}
