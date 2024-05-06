#include "HighscoreComponent.h"

#include <algorithm>
#include <iostream>
#include <SDL_events.h>

#include <fstream>
#include <sstream>

#include "Font.h"
#include "Renderer.h"
#include "TextComponent.h"
#include "GameObject.h"
#include "PlayerManager.h"
#include "PointComponent.h"
#include "ResourceManager.h"

using namespace dae;

HighscoreComponent::HighscoreComponent(GameObject* owner, const std::shared_ptr<Font>& font)
    :BaseComponent(owner),
    m_pNameText{ std::make_unique<TextComponent>("name", font, owner) }
{
}

void HighscoreComponent::Update(float deltaTime)
{
    m_pNameText->Update(deltaTime);
    EnterName(deltaTime);
}

void HighscoreComponent::Render() const
{
    m_pNameText->Render();
}

void HighscoreComponent::EnterName(float deltaTime)
{
    //Check if score isnt 0

    if (m_HasEnteredName) return;

    if (m_TimeBeforeWriting < 1)
    {
        m_TimeBeforeWriting += deltaTime;
        return;
    }

    std::string name{};
    bool quit = false;
    bool exit{};
    while (!quit)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT || e.type == SDLK_ESCAPE)
            {
                quit = true;
                exit = true;
            }
            if (e.type == SDL_KEYUP)
            {
                if (e.key.keysym.sym == SDLK_KP_ENTER || e.key.keysym.sym == SDLK_RETURN)
                {
                    if (!name.empty())
                        quit = true;
                }
                else if (e.key.keysym.sym == SDLK_BACKSPACE && !name.empty())
                {
                    name.pop_back();
                }
            }
            else if (e.type == SDL_TEXTINPUT && name.size() < 16)
            {
                name += e.text.text;
            }
            if (!name.empty())
                m_pNameText->SetText(name);
        }
        if (!name.empty())
        {
            m_pNameText->Update(deltaTime);
            m_pNameText->Render();
            Renderer::GetInstance().Render();
        }
    }

    m_HasEnteredName = true;
    if (exit) return;

    m_pNameText->SetText(name);

    const auto players = PlayerManager::GetInstance().GetPlayers();
    int score{};
    for (const auto& p : players)
    {
        score += p->GetComponent<PointComponent>()->GetAmount();
    }

    if (score > 0)
    {
        WriteToFile("../Data/HighScores.txt", name + ", " + std::to_string(score));
    }

    //Show names
    const auto highscores = GetHighscoreNames("../Data/HighScores.txt");
    auto smallFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);

    for (int i{}; i < static_cast<int>(highscores.size()); ++i)
    {
        if (i >= 10) return;

        const auto pointsObj = std::make_shared<dae::GameObject>();

        const std::string text = highscores[i].name + " - " + std::to_string(highscores[i].score);
        const auto pointsText = std::make_shared<dae::TextComponent>(text, smallFont, pointsObj.get());
        pointsObj->SetRelativePosition({ 340, -100 + i * 30 });
        pointsObj->AddComponent(pointsText);
        m_pOwner->AddChild(pointsObj);
    }


}

void HighscoreComponent::WriteToFile(const std::string& filename, const std::string& text)
{
    std::ifstream inputFile(filename);
    std::ofstream outputFile;

    if (inputFile.is_open())
    {
        std::string line;
        std::string fileContent;

        while (std::getline(inputFile, line))
        {
            fileContent += line + "\n";
        }

        inputFile.close();

        outputFile.open(filename);

        if (outputFile.is_open())
        {
            outputFile << fileContent << text;
            outputFile.close();
            std::cout << "Text written to file\n";
        }
    }
}

std::vector<HighscoreComponent::Highscore> HighscoreComponent::GetHighscoreNames(const std::string& filename)
{
    std::ifstream file(filename);
    std::string line;
    std::vector<Highscore> highscores;

    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string scoreStr, name;
        std::getline(iss, name, ',');
        std::getline(iss, scoreStr);
        int score = std::stoi(scoreStr);
        highscores.push_back({ name, score });
    }

    std::sort(highscores.begin(), highscores.end(), [](const Highscore& a, const Highscore& b)
        {
            return a.score > b.score;
        });

    for (const auto& record : highscores)
    {
        std::cout << record.name << " - " << record.score << std::endl;
    }

    return highscores;
}