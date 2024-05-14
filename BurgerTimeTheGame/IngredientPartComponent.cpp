#include "IngredientPartComponent.h"

#include "GameCollisionMngr.h"
#include "GameObject.h"

dae::IngredientPartComponent::IngredientPartComponent(dae::GameObject* owner)
	:BaseComponent(owner)
	,m_PushedDown(false)
	, m_collided(false)
{
}

void dae::IngredientPartComponent::Update(float)
{
	if(!m_PushedDown && m_collided)
	{
		m_PushedDown = true;
		glm::vec2 pos = m_pOwner->GetRelativePosition();
		pos.y += 3.f;
		m_pOwner->SetRelativePosition(pos);
	}

	if(!m_collided)
	m_collided = GameCollisionMngr::GetInstance().CheckOverlapWithPlayersBoolIngredients(m_pOwner->GetComponent<GameCollisionComponent>());
}
