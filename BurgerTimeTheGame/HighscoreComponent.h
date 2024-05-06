#pragma once
#include <memory>
#include <string>
#include <vector>

#include "BaseComponent.h"

namespace dae
{
	class Font;
	class TextComponent;
	class GameObject;


	class HighscoreComponent : public BaseComponent
	{
	public:

		HighscoreComponent(GameObject* owner, const std::shared_ptr<Font>& font);
		virtual ~HighscoreComponent() override = default;
		HighscoreComponent(const HighscoreComponent& other) = delete;
		HighscoreComponent(HighscoreComponent&& other) = delete;
		HighscoreComponent& operator=(const HighscoreComponent& other) = delete;
		HighscoreComponent& operator=(HighscoreComponent&& other) = delete;

		void Update(float deltaTime) override;
		void Render() const override;

	private:
		std::unique_ptr<dae::TextComponent> m_pNameText;

		float m_TimeBeforeWriting{};
		bool m_HasEnteredName{};
		struct Highscore
		{
			std::string name;
			int score;
		};
		
		void EnterName(float deltaTime);
		std::vector<Highscore> GetHighscoreNames(const std::string& filename);

		void WriteToFile(const std::string& filename, const std::string& text);

	};
}

