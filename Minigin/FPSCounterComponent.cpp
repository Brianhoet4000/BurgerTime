#include "FPSCounterComponent.h"
#include <stdexcept>
#include <SDL_ttf.h>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include <chrono>
#include "GameObject.h"

dae::FPSCounterComponent::FPSCounterComponent(const std::string& text, std::shared_ptr<Font> font, GameObject* owner)
	:BaseComponent(owner),
m_Text(text), m_Font(std::move(font)), m_TextTexture(nullptr)
{
	m_needsUpdate = true;
}

void dae::FPSCounterComponent::Update(float)
{
	if (m_needsUpdate)
	{
		const SDL_Color color = { 255,255,0 };
		const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_TextTexture = std::make_shared<Texture2D>(texture);
		m_needsUpdate = false;
	}

	static std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
	static int frame_count = 0;

	// Measure the elapsed time since the start time
	std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
	double elapsed_time = std::chrono::duration<double>(current_time - start_time).count();

	// Increment the frame count
	frame_count++;

	// Calculate the fps
	double fps = 1.0 / (elapsed_time / frame_count);

	// Reset the start time and frame count every second
	if (elapsed_time > 1.0) {
		start_time = current_time;
		frame_count = 0;
		m_needsUpdate = true;
	}
	m_Text = std::to_string(static_cast<int>(fps)) + " fps";
}

void dae::FPSCounterComponent::Render() const
{
	if (m_TextTexture != nullptr)
	{
		Renderer::GetInstance().RenderTexture(*m_TextTexture, m_pOwner->GetWorldPosition().x, m_pOwner->GetWorldPosition().y);
	}
}

void dae::FPSCounterComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_needsUpdate = true;
}




