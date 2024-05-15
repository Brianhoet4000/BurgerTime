#include "SpriteRenderComponent.h"

#include <glm/vec2.hpp>


#include "GameObject.h"


dae::SpriteRenderComponent::SpriteRenderComponent(GameObject* pOwner, SpriteSheet spriteSheet)
	: BaseComponent(pOwner)
	, m_SpriteSheet(std::move(spriteSheet))
{
	m_Rect.z = m_SpriteSheet.pTexture->GetSize().x / m_SpriteSheet.columns;
	m_Rect.w = m_SpriteSheet.pTexture->GetSize().y / m_SpriteSheet.rows;
}

void dae::SpriteRenderComponent::Update(float deltaTime)
{
	//m_RenderPos = m_pOwner().;

	if (m_Pause || m_Stop)
	{
		m_IsAnimationPlaying = false;
		return;
	}

	m_AccuTime += deltaTime;

	if (m_AccuTime > m_SpriteSheet.animTimer)
	{
		//UpdateCurrIdx
		++m_CurrIdx;
		if (m_PlaySelectedIdcs == false)
		{
			if (m_CurrIdx >= m_EndIdx)
			{
				++m_AccuLoops;

				if (m_Loops != -1 && m_AccuLoops > m_Loops)
				{
					m_Stop = true;
				}

				m_CurrIdx = m_StartIdx;
			}

			//Move rect
			m_Rect.x = (m_CurrIdx % m_SpriteSheet.columns) * m_Rect.z;
			m_Rect.y = (m_CurrIdx / m_SpriteSheet.columns) * m_Rect.w;
		}
		else
		{
			if (m_CurrIdx >= static_cast<int>(m_SelectedIdcs.size()))
			{
				++m_AccuLoops;

				if (m_Loops != -1 && m_AccuLoops > m_Loops)
				{
					m_Stop = true;
				}

				m_CurrIdx = 0;
			}

			m_Rect.x = (m_SelectedIdcs[m_CurrIdx] % m_SpriteSheet.columns) * m_Rect.z;
			m_Rect.y = (m_SelectedIdcs[m_CurrIdx] / m_SpriteSheet.columns) * m_Rect.w;
		}

		m_AccuTime = 0.f;
	}
}

void dae::SpriteRenderComponent::Render() const
{
	//glm::ivec4 dstRect;
	//dstRect.x = static_cast<int>(m_RenderPos.x);
	//dstRect.y = static_cast<int>(m_RenderPos.y);
	//dstRect.z = m_Rect.z;
	//dstRect.w = m_Rect.w;
	//
	//const glm::ivec2 rotationCenter{ dstRect.z / 2, dstRect.w / 2 };
	//
	//Locator::GetRenderSystem().RenderSprite(*m_SpriteSheet.pTexture, m_Rect, dstRect, 0, rotationCenter, m_Flip);
}

void dae::SpriteRenderComponent::SelectSprite(int idx)
{
	m_Pause = true;

	m_Rect.x = (idx % m_SpriteSheet.columns) * m_Rect.z;
	m_Rect.y = (idx / m_SpriteSheet.columns) * m_Rect.w;
}

void dae::SpriteRenderComponent::PlayAnimation(int startIdx, int endIdx, int loops)
{
	m_AccuTime = 0.f;

	m_StartIdx = startIdx;
	m_EndIdx = endIdx;
	m_CurrIdx = m_StartIdx;

	m_Loops = loops;

	m_Pause = false;
	m_Stop = false;
	m_IsAnimationPlaying = true;

	//m_Flip = RenderFlip::none;
}

void dae::SpriteRenderComponent::PlayAnimation(const std::vector<int>& idcs, int loops)
{
	m_PlaySelectedIdcs = true;
	m_SelectedIdcs = idcs;

	m_Loops = loops;

	m_CurrIdx = 0;

	m_Pause = false;
	m_Stop = false;
	m_IsAnimationPlaying = true;

	//m_Flip = RenderFlip::none;
}

void dae::SpriteRenderComponent::UpdateAnimation(int startIdx, int endIdx, int loops)
{
	m_AccuTime = 0.f;

	m_StartIdx = startIdx;
	m_EndIdx = endIdx;
	m_CurrIdx = m_StartIdx;

	m_Loops = loops;
}

void dae::SpriteRenderComponent::Pause(bool value)
{
	m_Pause = value;
}

void dae::SpriteRenderComponent::Stop(bool value)
{
	m_Stop = value;

	if (m_Stop)
		m_IsAnimationPlaying = false;
}

/*
void dae::SpriteRenderComponent::FlipTexture(RenderFlip flip)
{
	m_Flip = flip;
}
*/

glm::ivec2 dae::SpriteRenderComponent::GetSpriteSize() const
{
	return glm::ivec2(m_Rect.z, m_Rect.w);
}