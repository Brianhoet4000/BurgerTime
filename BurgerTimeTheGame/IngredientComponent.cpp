#include "IngredientComponent.h"

#include "GameCollisionMngr.h"
#include "GameObject.h"
#include "IngredientPartComponent.h"

dae::IngredientComponent::IngredientComponent(dae::GameObject* owner)
	:BaseComponent(owner)
	,m_allPushedDown(false)
	,m_IsFalling(false)
	,m_DoOnce(false)
{

}

void dae::IngredientComponent::Update(float deltaTime)
{
	if(m_IsFalling)
	{
		for (const auto& element : m_pOwner->GetChildren())
		{
			if (element == nullptr) return;

			const auto& childCollider = element->GetComponent<GameCollisionComponent>();
			GameCollisionComponent* currentFloor = nullptr;

			if (!m_DoOnce)
			{
				currentFloor = dae::GameCollisionMngr::GetInstance().GetCurrentFloor(childCollider);
				m_DoOnce = true;
			}

			if (dae::GameCollisionMngr::GetInstance().CheckOverlapIngredientsWithFloors(childCollider, currentFloor))
			{
				element->GetComponent<IngredientPartComponent>()->SetPushedDown(false);
				m_IsFalling = false;
				m_DoOnce = false;
			}
			glm::vec2 pos = element->GetRelativePosition();
			pos.y += m_speed * deltaTime;
			element->SetRelativePosition(pos.x, pos.y);
		}
	}

	if(m_allPushedDown && !m_IsFalling)
	{
		m_IsFalling = true;
	}
	else
	{
		m_allPushedDown = AllArePushedDown();
	}
		
}

bool dae::IngredientComponent::AllArePushedDown() const
{
	for (auto& element : m_pOwner->GetChildren())
	{
		if (element == nullptr) return false;

		if (!element.get()->GetComponent<IngredientPartComponent>()->GetPushedDown())
		{
			return false;
		}
	}
	return true;
}
