#include "PlateComponent.h"

#include "GameCollisionMngr.h"
#include "GameObject.h"

dae::PlateComponent::PlateComponent(dae::GameObject* owner)
	:BaseComponent(owner)
	,m_AmountOfIngredients{0}
	,m_PlateFull{false}
{
}

void dae::PlateComponent::Update(float)
{
	if (m_AmountOfIngredients >= 4)
	{
		m_PlateFull = true;
		std::cout << "Full\n";
	}
	dae::GameCollisionMngr::GetInstance().CheckOverlapIngredientsWithPlates(m_pOwner->GetComponent<dae::GameCollisionComponent>(), m_AmountOfIngredients);
}
