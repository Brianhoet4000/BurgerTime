#include "PlateComponent.h"

#include "GameCollisionMngr.h"
#include "GameObject.h"

dae::PlateComponent::PlateComponent(dae::GameObject* owner)
	:BaseComponent(owner)
	,m_AmountOfIngredients{0}
{
}

void dae::PlateComponent::Update(float)
{
	dae::GameCollisionMngr::GetInstance().CheckOverlapIngredientsWithPlates(m_pOwner->GetComponent<dae::GameCollisionComponent>(), m_AmountOfIngredients);
}
