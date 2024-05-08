#include "GameCollisionComponent.h"

#include "GameCollisionMngr.h"
#include "GameObject.h"
#include "Renderer.h"
#include "TextureComponent.h"

dae::GameCollisionComponent::GameCollisionComponent(dae::GameObject* owner, glm::vec2 pos, const float width, const float height, bool needsUpdate, bool visualizeRect, bool IsVersus)
	:BaseComponent(owner)
	,m_Pos(pos)
	,m_NeedsUpdate(needsUpdate)
	,m_RenderCollisionBox(visualizeRect)
	,m_IsVersus{ IsVersus }
{
	m_CollisionBox = SDL_FRect{ pos.x, pos.y, width, height};

	GameCollisionMngr::GetInstance().AddCollisionBox(m_pOwner, this);
}

void dae::GameCollisionComponent::Update(float)
{
	if (!m_NeedsUpdate) return;

	m_CollisionBox.x = m_pOwner->GetRelativePosition().x + m_Offset.x;
	m_CollisionBox.y = m_pOwner->GetRelativePosition().y + m_Offset.y;
}

void dae::GameCollisionComponent::Render() const
{
	if (m_RenderCollisionBox)
	{
		SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), m_Color.r, m_Color.g, m_Color.b, m_Color.a);
		SDL_RenderDrawRectF(Renderer::GetInstance().GetSDLRenderer(), &m_CollisionBox);
	}
}

SDL_FRect dae::GameCollisionComponent::GetCollisionRect() const
{
	return m_CollisionBox;
}

void dae::GameCollisionComponent::SetCollisionRectOffset(float Offset)
{
	m_Offset = glm::vec2{Offset, Offset};
	m_CollisionBox.w = m_pOwner->GetComponent<dae::TextureComponent>()->GetSize().x - (Offset * 2.0f);
	m_CollisionBox.h = m_pOwner->GetComponent<dae::TextureComponent>()->GetSize().y - (Offset * 2.0f);
}

void dae::GameCollisionComponent::SetCollisionRectOffset(float Offsetx, float Offsety, bool centerizedOffsetX, bool centerizedOffsetY)
{
	m_Offset = glm::vec2{ Offsetx, Offsety };

	// Calculate the new position of the collision box
	if (!centerizedOffsetX)
	{
		m_CollisionBox.x = m_pOwner->GetRelativePosition().x + Offsetx;
	}
	else
	{
		m_CollisionBox.x = m_pOwner->GetComponent<dae::TextureComponent>()->GetSize().x - (Offsetx * 2.0f);
		m_CollisionBox.w = m_CollisionBox.w - (Offsetx * 2.0f);
	}
	if(!centerizedOffsetY)
	{
		m_CollisionBox.y = m_pOwner->GetRelativePosition().y + Offsety;
	}
	else
	{
		m_CollisionBox.y = m_pOwner->GetComponent<dae::TextureComponent>()->GetSize().y - (Offsety * 2.0f);
		m_CollisionBox.h = m_CollisionBox.h - (Offsety * 2.0f);
	}
}


dae::GameObject* dae::GameCollisionComponent::GetOwner() const
{
	return m_pOwner;
}