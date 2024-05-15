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
			m_Collided = pushedDown;
		}

		bool GetCollided() const { return m_Collided; }
		void SetCollided(bool collided)
		{
			m_Collided = collided;
		}

	private:
		bool m_PushedDown;
		bool m_Collided;
	};

}