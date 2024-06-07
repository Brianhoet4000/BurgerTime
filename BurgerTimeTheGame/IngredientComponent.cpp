#include "IngredientComponent.h"

#include "GameCollisionMngr.h"
#include "GameObject.h"
#include "IngredientPartComponent.h"
#include "PointComponent.h"
#include "ScreenManager.h"
#include "ServiceLocator.h"
#include "SoundSystem.h"

dae::IngredientComponent::IngredientComponent(dae::GameObject* owner)
	:BaseComponent(owner)
	,m_allPushedDown(false)
	,m_IsFalling(false)
	,m_Completed(false)
{

}

void dae::IngredientComponent::Update(float deltaTime)
{
	if (m_Completed) return;

	if(m_IsFalling)
	{
		for (size_t i = 0; i < m_pOwner->GetChildren().size(); ++i)
		{
			if (m_pOwner->GetChildren()[i] == nullptr) return;

			const auto& childCollider = m_pOwner->GetChildren()[i]->GetComponent<GameCollisionComponent>();

			GameCollisionMngr::GetInstance().CheckIngredientOverlapWithEnemies(childCollider);

			//std::cout << "implement secondplayer in versus\n";
			//GameCollisionMngr::GetInstance().CheckIngredientOverlapWithSecondplayer(childCollider);

			GameCollisionMngr::GetInstance().CheckOverlapIngredientsWithOtherIngredients(childCollider);

			if (dae::GameCollisionMngr::GetInstance().CheckOverlapIngredientsWithFloors(childCollider))
			{
				m_pOwner->GetChildren()[i]->GetComponent<IngredientPartComponent>()->SetPushedDown(false);
				m_pOwner->GetChildren()[i]->GetComponent<IngredientPartComponent>()->SetCollided(false);
				m_IsFalling = false;
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

bool dae::IngredientComponent::AllArePushedDown()
{
	for (auto& element : m_pOwner->GetChildren())
	{
		if (element == nullptr) return false;

		if (!element.get()->GetComponent<IngredientPartComponent>()->GetPushedDown())
		{
			return false;
		}
	}

	const auto& player = m_pOwner->GetChildren()[0]->GetComponent<IngredientPartComponent>()->GetPlayer();
	dae::ScreenManager::GetInstance().IncreasePoint(player, 50);

	return true;
}
