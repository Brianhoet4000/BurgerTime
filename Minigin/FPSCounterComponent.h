#pragma once
#include <memory>
#include <string>
#include "BaseComponent.h"

namespace dae
{
	class Font;
	class Texture2D;
	class FPSCounterComponent final : public BaseComponent
	{
	public:
		virtual void Update(float) override;
		virtual void Render() const;

		void SetText(const std::string& text);

		FPSCounterComponent(const std::string& text, std::shared_ptr<Font> font, GameObject* owner);
		virtual ~FPSCounterComponent() = default;
		FPSCounterComponent(const FPSCounterComponent& other) = delete;
		FPSCounterComponent(FPSCounterComponent&& other) = delete;
		FPSCounterComponent& operator=(const FPSCounterComponent& other) = delete;
		FPSCounterComponent& operator=(FPSCounterComponent&& other) = delete;
	private:
		std::string m_Text;
		std::shared_ptr<Font> m_Font;
		std::shared_ptr<Texture2D> m_TextTexture;
	};
}