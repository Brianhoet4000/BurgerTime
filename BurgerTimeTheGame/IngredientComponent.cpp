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
		for (size_t i = 0; i < m_pOwner->GetChildren().size(); ++i)
		{
			if (m_pOwner->GetChildren()[i] == nullptr) return;

			const auto& childCollider = m_pOwner->GetChildren()[i]->GetComponent<GameCollisionComponent>();

			if(GameCollisionMngr::GetInstance().CheckOverlapIngredientsWithOtherIngredients(childCollider))
			{
				m_IsFalling = true;
			}
			

			if (dae::GameCollisionMngr::GetInstance().CheckOverlapIngredientsWithFloors(childCollider))
			{
				m_IsFalling = false;
				m_DoOnce = false;
				m_pOwner->GetChildren()[i]->GetComponent<IngredientPartComponent>()->SetPushedDown(false);
				float y = m_pOwner->GetChildren()[0]->GetRelativePosition().y;
				m_pOwner->GetChildren()[i]->SetRelativePosition(m_pOwner->GetChildren()[i]->GetRelativePosition().x, y);
			}

			glm::vec2 pos = m_pOwner->GetChildren()[i]->GetRelativePosition();
			pos.y += m_speed * deltaTime;
			m_pOwner->GetChildren()[i]->SetRelativePosition(pos.x, pos.y);
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
