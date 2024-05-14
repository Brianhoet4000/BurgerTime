#include "IngredientComponent.h"

#include "GameObject.h"
#include "IngredientPartComponent.h"

dae::IngredientComponent::IngredientComponent(dae::GameObject* owner)
	:BaseComponent(owner)
	,m_allPushedDown(false)
{

}

void dae::IngredientComponent::Update(float deltaTime)
{
	if(m_allPushedDown)
	{
		for (const auto& element : m_pOwner->GetChildren())
		{
			if (element == nullptr) return;

			glm::vec2 pos = element->GetRelativePosition();
			pos.y += m_speed * deltaTime;
			element->SetRelativePosition(pos.x, pos.y);
		}
	}
	else
	{
		m_allPushedDown = AllArePushedDown();
	}
		
}

bool dae::IngredientComponent::AllArePushedDown() const
{
	for (const auto& element : m_pOwner->GetChildren())
	{
		if (element == nullptr) return false;

		if (!element.get()->GetComponent<IngredientPartComponent>()->GetPushedDown())
		{
			return false;
		}
	}
	return true;
}
