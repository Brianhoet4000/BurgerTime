#include "BulletTimerComponent.h"
#include "GameObject.h"
#include "TextureComponent.h"

dae::BulletTimerComponent::BulletTimerComponent(dae::GameObject* owner, const float timer)
	:BaseComponent(owner)
	,m_StartValue{ timer }
	,m_CurrentTime{m_StartValue}

{
}

void dae::BulletTimerComponent::Update(float deltaTime)
{
	if(m_HasShot)
	{
		m_CurrentTime -= 1 * deltaTime;

		if(m_CurrentTime <= 0)
		{
			m_HasShot = false;
			m_Completed = true;
			m_CurrentTime = m_StartValue;
		}
	}
}
