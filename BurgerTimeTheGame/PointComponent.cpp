#include "PointComponent.h"

dae::PointComponent::PointComponent(dae::GameObject* owner, int startamount)
	:BaseComponent(owner)
{
	m_pCounterCp = std::make_unique<dae::CounterComponent>(owner, startamount);
}

void dae::PointComponent::DecreaseAmount(int difference) const
{
	m_pCounterCp->DecreaseAmount(difference);
}

void dae::PointComponent::IncreaseAmount(int difference) const
{
	m_pCounterCp->IncreaseAmount(difference);
}

void dae::PointComponent::SetAmount(int newInt) const
{
	m_pCounterCp->SetAmount(newInt);
}
