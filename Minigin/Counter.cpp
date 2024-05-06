#pragma once
#include "Counter.h"
#include "Font.h"
#include "Texture2D.h"
#include "Renderer.h"
#include "GameObject.h"
#include "TextComponent.h"



using namespace dae;

CounterComponent::CounterComponent(GameObject* owner, int startAmount)
	: BaseComponent(owner),
	m_Counter{ startAmount }
{
	
}

void CounterComponent::IncreaseAmount(int difference)
{
	m_Counter += difference;
}

void dae::CounterComponent::DecreaseAmount(int difference)
{
	m_Counter -= difference;
}

void CounterComponent::SetAmount(int newAmount)
{
	m_Counter = newAmount;
}

int CounterComponent::GetAmount() const
{
	return m_Counter;
}

