#pragma once
#include <string>
#include <memory>
#include "Transform.h"
#include "BaseComponent.h"

namespace dae
{
	class Font;
	class Texture2D;
	class TextComponent final: public BaseComponent
	{
	public:
		virtual void Update(float) override;
		virtual void Render() const override;

		void SetText(const std::string& text);
		std::string GetBeginText() const { return m_BeginText; }

		TextComponent(const std::string& text, std::shared_ptr<Font> font, GameObject* owner);
		virtual ~TextComponent() = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;
	private:
		std::string m_Text, m_BeginText{};;
		std::shared_ptr<Font> m_Font;
		std::shared_ptr<Texture2D> m_TextTexture;
	};
}
