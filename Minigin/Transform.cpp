#include "Transform.h"

#include "GameObject.h"

dae::Transform::Transform(GameObject* owner)
	:BaseComponent(owner)
	,m_position(0,0)
{
	
}

void dae::Transform::SetPosition(const float x, const float y)
{
	m_position.x = x;
	m_position.y = y;
}

//void dae::Transform::Update(float)
//{
//	m_position = m_pOwner->GetWorldPosition();
//}
