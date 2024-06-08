#include "RandomSpawn.h"

dae::SpawnComponent::SpawnComponent(dae::GameObject* owner, glm::vec2 pos)
	:BaseComponent(owner)
	,m_SpawnPos(pos)
{
	
}

glm::vec2 dae::SpawnComponent::GetPosition() const
{
	return m_SpawnPos;
}
