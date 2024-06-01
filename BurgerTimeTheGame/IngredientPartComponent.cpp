#include "IngredientPartComponent.h"

#include "GameCollisionMngr.h"
#include "GameObject.h"
#include "IngredientComponent.h"
#include "ServiceLocator.h"

dae::IngredientPartComponent::IngredientPartComponent(dae::GameObject* owner)
	:BaseComponent(owner)
	,m_PushedDown(false)
	,m_Collided(false)
{
}

void dae::IngredientPartComponent::Update(float)
{
	if (m_pOwner->GetParent()->GetComponent<IngredientComponent>()->GetIsFalling()) return;

	if (!m_PushedDown && m_Collided)
	{
		glm::vec2 pos = m_pOwner->GetRelativePosition();
		pos.y += 3;
		m_pOwner->SetRelativePosition(pos);
		m_PushedDown = true;
	}
	else if (!m_Collided)
	{
		m_Collided = GameCollisionMngr::GetInstance().CheckOverlapWithPlayersBoolIngredients(m_pOwner->GetComponent<GameCollisionComponent>());
	}
}
