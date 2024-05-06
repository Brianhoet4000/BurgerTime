#include "TextureComponent.h"
#include "Renderer.h"
#include "GameObject.h"
#include "ResourceManager.h"

namespace dae
{

	TextureComponent::TextureComponent(GameObject* owner)
		:BaseComponent(owner)
	{
		
	}

	glm::vec2 TextureComponent::GetSize()
	{
		return m_Texture->GetSize();
	}

	void TextureComponent::SetMustRender(bool render)
	{
		m_MustTexture = render;
	}

	bool TextureComponent::GetMustRender() const
	{
		return m_MustTexture;
	}

	void TextureComponent::Update(float)
	{
	}

	void TextureComponent::Render() const
	{
		if (!m_MustTexture) return;

		dae::Renderer::GetInstance().RenderTexture(*m_Texture, m_pOwner->GetWorldPosition().x, m_pOwner->GetWorldPosition().y);
	}

	void TextureComponent::SetTexture(const std::string& filename)
	{
		m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
	}
}