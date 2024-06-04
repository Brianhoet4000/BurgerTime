#include "HealthComponent.h"

#include "Scene.h"
#include "SceneManager.h"

dae::HealthComponent::HealthComponent(dae::GameObject* owner, int startAmount)
	:BaseComponent(owner)
{
	m_pCounterCp = std::make_unique<dae::CounterComponent>(owner, startAmount);
}

void dae::HealthComponent::DecreaseAmount(int difference) const
{
	m_pCounterCp->DecreaseAmount(difference);

	if(m_pCounterCp->GetAmount() < -1)
	{
		dae::SceneManager::GetInstance().GetActiveScene()->RemoveAll();
		dae::SceneManager::GetInstance().SetActiveScene("GameOver");
		return;
	}
}

void dae::HealthComponent::IncreaseAmount(int difference) const
{
	m_pCounterCp->IncreaseAmount(difference);
}

void dae::HealthComponent::SetAmount(int newHealth) const
{
	m_pCounterCp->SetAmount(newHealth);
}
