#pragma once
#include <memory>
#include <vector>
#include "GameObject.h"
#include "GameCollisionComponent.h"

namespace dae
{

    class PlayerManager final
    {
    public:

        ~PlayerManager() = default;
        PlayerManager(const PlayerManager&) = delete;
        PlayerManager& operator=(const PlayerManager&) = delete;
        PlayerManager(PlayerManager&&) = default;
        PlayerManager& operator=(PlayerManager&&) = default;

        static PlayerManager& GetInstance()
        {
            static PlayerManager instance;
            return instance;
        }

        void AddPlayer(const std::shared_ptr<dae::GameObject>& player)
        {
            m_Players.push_back(player);
        }
        void RemovePlayer(const std::shared_ptr<dae::GameObject>& player)
        {
            m_Players.erase(std::remove(m_Players.begin(), m_Players.end(), player), m_Players.end());
        }
        void RemoveAllPlayers()
        {
            m_Players.clear();
        }
        std::vector<std::shared_ptr<dae::GameObject>> GetPlayers() { return m_Players; }
        std::vector<std::shared_ptr<dae::GameObject>> GetPlayersNotVersus() const
        {
            std::vector<std::shared_ptr<dae::GameObject>> players;

            for (const auto& player : m_Players)
            {
                if (!player->GetComponent<dae::GameCollisionComponent>()->GetIsVersus())
                {
                    players.push_back(player);
                }
            }
            return players;
        }

    private:
        PlayerManager() = default;

        std::vector<std::shared_ptr<dae::GameObject>> m_Players;
    };
}
