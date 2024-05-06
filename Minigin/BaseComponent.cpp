#include "BaseComponent.h"
#include"GameObject.h"

namespace dae
{

	BaseComponent::BaseComponent(GameObject* owner)
		:m_needsUpdate{ true }
	{
		m_pOwner = owner;
	}

	BaseComponent::~BaseComponent()
	{
	}

	void BaseComponent::Update(float)
	{
	}

	void BaseComponent::FixedUpdate(float)
	{
	}

	void BaseComponent::Render() const
	{

	}
}