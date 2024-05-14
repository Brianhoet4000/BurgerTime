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
			std::cout << "changed value\n";
			m_PushedDown = pushedDown;
		}

	private:
		bool m_PushedDown;
		bool m_collided;
	};

}
