#include "GameCollisionMngr.h"
#include "GameObject.h"
#include "HobbinComponent.h"
#include "PlayerManager.h"
#include "PointComponent.h"
#include "ScreenManager.h"
#include "ServiceLocator.h"

namespace dae
{
    class GoldStateComponent;

    void dae::GameCollisionMngr::AddCollisionBox(dae::GameObject* owner, GameCollisionComponent* box)
    {
        m_pCollisonBoxes.push_back(box);

        //Check which tag
        {
        	if (owner->GetTag() == "Wall")
            {
                m_pWallBoxes.push_back(box);
            }
            else if (owner->GetTag() == "Stair")
            {
                m_pStairsBoxes.push_back(box);
            }
            else if (owner->GetTag() == "Floor")
            {
                m_pFloorBoxes.push_back(box);
            }
            else if (owner->GetTag() == "Bullet")
            {
                m_pBulletBoxes.push_back(box);
            }
            else if (owner->GetTag() == "Enemy")
            {
                m_pEnemies.push_back(box);
            }
            else if (owner->GetTag() == "Player_01")
            {
                m_pFirstPlayer = box;
            }
            else if (owner->GetTag() == "Player_02")
            {
                m_pSecondPlayer = box;
            }
        }
    }

    void dae::GameCollisionMngr::RemoveCollisionBox(GameCollisionComponent* box)
    {
        m_pCollisonBoxes.erase(std::remove(m_pCollisonBoxes.begin(), m_pCollisonBoxes.end(), box), m_pCollisonBoxes.end());
    }
    void GameCollisionMngr::RemoveBulletBox(GameCollisionComponent* box)
    {
        RemoveCollisionBox(box);
        m_pBulletBoxes.erase(std::remove(m_pBulletBoxes.begin(), m_pBulletBoxes.end(), box), m_pBulletBoxes.end());
    }
    void GameCollisionMngr::RemoveFirstPlayerBox(GameCollisionComponent* box)
    {
        m_pFirstPlayer = nullptr;
        RemoveCollisionBox(box);
    }
    void GameCollisionMngr::RemoveSecondPlayerBox(GameCollisionComponent* box)
    {
        m_pSecondPlayer = nullptr;
        RemoveCollisionBox(box);
    }
    void GameCollisionMngr::RemoveEnemyBox(GameCollisionComponent* box)
    {
        RemoveCollisionBox(box);
        m_pEnemies.erase(std::remove(m_pEnemies.begin(), m_pEnemies.end(), box), m_pEnemies.end());
    }

    void GameCollisionMngr::ClearAll()
    {
        m_pCollisonBoxes.clear();
        m_pWallBoxes.clear();
        m_pStairsBoxes.clear();
        m_pFloorBoxes.clear();
        m_pBulletBoxes.clear();
        m_pEnemies.clear();
        m_pFirstPlayer = nullptr;
        m_pSecondPlayer = nullptr;
    }

    std::vector<dae::GameCollisionComponent*> dae::GameCollisionMngr::GetAllWall()
    {
        return m_pWallBoxes;
    }
    std::vector<dae::GameCollisionComponent*> dae::GameCollisionMngr::GetAllStairs()
    {
        return m_pStairsBoxes;
    }
    std::vector<dae::GameCollisionComponent*> dae::GameCollisionMngr::GetAllFloors()
    {
        return m_pFloorBoxes;
    }
    std::vector<dae::GameCollisionComponent*> GameCollisionMngr::GetAllEnemies()
    {
        return m_pEnemies;
    }

    std::vector<GameCollisionComponent*> GameCollisionMngr::GetAllPlayers() const
    {
        std::vector<dae::GameCollisionComponent*> pPlayers;

        pPlayers.push_back(m_pFirstPlayer);

        if (m_pSecondPlayer != nullptr)
        {
            if (!m_pSecondPlayer->GetIsVersus())
            {
                pPlayers.push_back(m_pSecondPlayer);
            }
        }

        return pPlayers;
    }

    bool dae::GameCollisionMngr::CheckForCollision(const GameCollisionComponent* box) const
    {
        for (const auto& otherBox : m_pCollisonBoxes)
        {
            if (otherBox == box)
                continue;

            if (box->GetCollisionRect().x < otherBox->GetCollisionRect().x + otherBox->GetCollisionRect().w &&
                box->GetCollisionRect().x + box->GetCollisionRect().w > otherBox->GetCollisionRect().x &&
                box->GetCollisionRect().y < otherBox->GetCollisionRect().y + otherBox->GetCollisionRect().h &&
                box->GetCollisionRect().y + box->GetCollisionRect().h > otherBox->GetCollisionRect().y)
            {
                return true;
            }
        }

        return false;
    }

    dae::GameCollisionComponent* dae::GameCollisionMngr::CheckForCollisionComponent(const GameCollisionComponent* box) const
    {
        for (const auto& otherBox : m_pCollisonBoxes)
        {
            if (otherBox == box)
                continue;

            if (box->GetCollisionRect().x < otherBox->GetCollisionRect().x + otherBox->GetCollisionRect().w &&
                box->GetCollisionRect().x + box->GetCollisionRect().w > otherBox->GetCollisionRect().x &&
                box->GetCollisionRect().y < otherBox->GetCollisionRect().y + otherBox->GetCollisionRect().h &&
                box->GetCollisionRect().y + box->GetCollisionRect().h > otherBox->GetCollisionRect().y)
            {
                return otherBox;
            }
        }
        return nullptr;
    }

    GameCollisionComponent* GameCollisionMngr::CheckOverlapWithEnemiesComponent(const GameCollisionComponent* box) const
    {
        if (!m_pEnemies.empty())
        {
            for (const auto& otherBox : m_pEnemies)
            {
                if (otherBox == box)
                    continue;

                if (box->GetCollisionRect().x < otherBox->GetCollisionRect().x + otherBox->GetCollisionRect().w &&
                    box->GetCollisionRect().x + box->GetCollisionRect().w > otherBox->GetCollisionRect().x &&
                    box->GetCollisionRect().y < otherBox->GetCollisionRect().y + otherBox->GetCollisionRect().h &&
                    box->GetCollisionRect().y + box->GetCollisionRect().h > otherBox->GetCollisionRect().y)
                {
                    return otherBox;
                }
            }
        }
        return nullptr;
    }

    GameCollisionComponent* GameCollisionMngr::CheckOverlapWithSecondPlayerVersus(const GameCollisionComponent* box) const
    {
        for (const auto& player : PlayerManager::GetInstance().GetPlayers())
        {
            const auto& pPlayerCollision = player->GetComponent<dae::GameCollisionComponent>();
            if (pPlayerCollision->GetIsVersus())
            {
                if (box->GetCollisionRect().x < pPlayerCollision->GetCollisionRect().x + pPlayerCollision->GetCollisionRect().w &&
                    box->GetCollisionRect().x + box->GetCollisionRect().w > pPlayerCollision->GetCollisionRect().x &&
                    box->GetCollisionRect().y < pPlayerCollision->GetCollisionRect().y + pPlayerCollision->GetCollisionRect().h &&
                    box->GetCollisionRect().y + box->GetCollisionRect().h > pPlayerCollision->GetCollisionRect().y)
                {
                    return pPlayerCollision;
                }
            }
        }
        return nullptr;
    }

    GameCollisionComponent* GameCollisionMngr::CheckOverlapWithFirstPlayer(const GameCollisionComponent* box) const
    {
    	const auto& player = PlayerManager::GetInstance().GetPlayers()[0]->GetComponent<GameCollisionComponent>();
		if (player != nullptr)
		{
			if (box->GetCollisionRect().x < player->GetCollisionRect().x + player->GetCollisionRect().w &&
               box->GetCollisionRect().x + box->GetCollisionRect().w > player->GetCollisionRect().x &&
               box->GetCollisionRect().y < player->GetCollisionRect().y + player->GetCollisionRect().h &&
               box->GetCollisionRect().y + box->GetCollisionRect().h > player->GetCollisionRect().y)
			{
				return player;
			}
		}
        
        return nullptr;
    }

    GameCollisionComponent* GameCollisionMngr::CheckOverlapWithPlayers(const GameCollisionComponent* box) const
    {
        for (const auto& player : PlayerManager::GetInstance().GetPlayers())
        {
            if (player == nullptr) return nullptr;

            const auto& PlayerBox = player->GetComponent<GameCollisionComponent>();

            if (box->GetCollisionRect().x < PlayerBox->GetCollisionRect().x + PlayerBox->GetCollisionRect().w &&
                box->GetCollisionRect().x + box->GetCollisionRect().w > PlayerBox->GetCollisionRect().x &&
                box->GetCollisionRect().y < PlayerBox->GetCollisionRect().y + PlayerBox->GetCollisionRect().h &&
                box->GetCollisionRect().y + box->GetCollisionRect().h > PlayerBox->GetCollisionRect().y)
            {
                return PlayerBox;
            }
        }
       
        return nullptr;
    }

    bool GameCollisionMngr::CheckOverlapWithPlayersBool(const GameCollisionComponent* box) const
    {
        for (const auto& player : PlayerManager::GetInstance().GetPlayers())
        {
            if (player == nullptr) return false;
            if (player->ReturnDeleting()) return false;

            const auto& PlayerBox = player->GetComponent<GameCollisionComponent>();

            if (box->GetCollisionRect().x < PlayerBox->GetCollisionRect().x + PlayerBox->GetCollisionRect().w &&
                box->GetCollisionRect().x + box->GetCollisionRect().w > PlayerBox->GetCollisionRect().x &&
                box->GetCollisionRect().y < PlayerBox->GetCollisionRect().y + PlayerBox->GetCollisionRect().h &&
                box->GetCollisionRect().y + box->GetCollisionRect().h > PlayerBox->GetCollisionRect().y)
            {
                return true;
            }
        }

        return false;
    }

    bool GameCollisionMngr::CheckForInStairsX() const
    {
        constexpr int offsetX = 1; // Adjust as needed

        for (const auto& stairs : m_pStairsBoxes)
        {
            if (stairs == nullptr)
                return false;

            for (const auto& player : PlayerManager::GetInstance().GetPlayers())
            {
                const auto& PlayerBox = player->GetComponent<GameCollisionComponent>();

                // Adjust the comparison with the offset
                if (PlayerBox->GetCollisionRect().x + offsetX >= stairs->GetCollisionRect().x &&
                    PlayerBox->GetCollisionRect().x + PlayerBox->GetCollisionRect().w - offsetX <= stairs->GetCollisionRect().x + stairs->GetCollisionRect().w)
                {
                    return true;
                }
            }
        }
        return false;
    }

    bool GameCollisionMngr::CheckOverlapWithStairs(const glm::vec2&) const
    {

        for (const auto& stairs : m_pStairsBoxes)
        {
            if (stairs == nullptr)
                return false;

            for (const auto& player : PlayerManager::GetInstance().GetPlayers())
            {
                const auto& PlayerBox = player->GetComponent<GameCollisionComponent>();

                if (PlayerBox->GetCollisionRect().x >= stairs->GetCollisionRect().x &&
                    PlayerBox->GetCollisionRect().x + PlayerBox->GetCollisionRect().w <= stairs->GetCollisionRect().x + stairs->GetCollisionRect().w &&
                    PlayerBox->GetCollisionRect().y >= stairs->GetCollisionRect().y &&
                    PlayerBox->GetCollisionRect().y + PlayerBox->GetCollisionRect().h <= stairs->GetCollisionRect().y + stairs->GetCollisionRect().h)
                {
                    return true;
                }
            }
        }
        return false;
    }

    bool GameCollisionMngr::MovePlayerUpStairs() const
    {
        for (const auto& stairs : m_pStairsBoxes)
        {
            if (stairs == nullptr)
                return false;

            for (const auto& player : PlayerManager::GetInstance().GetPlayers())
            {
                const auto& playerBox = player->GetComponent<GameCollisionComponent>();
                // Check if player can move up the stairs
                // Adjusting for top-left origin by comparing top edge of player with bottom edge of stairs
                if (playerBox->GetCollisionRect().x >= stairs->GetCollisionRect().x &&
                    playerBox->GetCollisionRect().x + playerBox->GetCollisionRect().w <= stairs->GetCollisionRect().x + stairs->GetCollisionRect().w &&
                    playerBox->GetCollisionRect().y < stairs->GetCollisionRect().y && playerBox->GetCollisionRect().y > stairs->GetCollisionRect().y - 10) // Going up
                {
                    return true;
                }
            }
        }
        return false;
    }


    bool GameCollisionMngr::MovePlayerDownStairs() const
    {
        for (const auto& stairs : m_pStairsBoxes)
        {
            if (stairs == nullptr)
                return false;

            for (const auto& player : PlayerManager::GetInstance().GetPlayers())
            {
                const auto& playerBox = player->GetComponent<GameCollisionComponent>();

            	if (playerBox->GetCollisionRect().x >= stairs->GetCollisionRect().x &&
                    playerBox->GetCollisionRect().x + playerBox->GetCollisionRect().w <= stairs->GetCollisionRect().x + stairs->GetCollisionRect().w &&
                    playerBox->GetCollisionRect().y + playerBox->GetCollisionRect().h > stairs->GetCollisionRect().y + stairs->GetCollisionRect().h &&
                    playerBox->GetCollisionRect().y + playerBox->GetCollisionRect().h < stairs->GetCollisionRect().y + stairs->GetCollisionRect().h + 10)
                {
                    return true;
                }
            }
        }
        return false;
    }

    bool GameCollisionMngr::CheckOverlapWithFloors() const
    {
        // Define offset values
        constexpr int offsetX = 2; // Adjust as needed
        constexpr int offsetY = 2; // Adjust as needed

        for (const auto& floors : m_pFloorBoxes)
        {
            if (floors == nullptr)
                return false;

            for (const auto& player : PlayerManager::GetInstance().GetPlayers())
            {
                const auto& PlayerBox = player->GetComponent<GameCollisionComponent>();

                // Adjust the comparison with the offset
                if (PlayerBox->GetCollisionRect().x + offsetX >= floors->GetCollisionRect().x &&
                    PlayerBox->GetCollisionRect().x + PlayerBox->GetCollisionRect().w - offsetX <= floors->GetCollisionRect().x + floors->GetCollisionRect().w &&
                    PlayerBox->GetCollisionRect().y + offsetY >= floors->GetCollisionRect().y &&
                    PlayerBox->GetCollisionRect().y + PlayerBox->GetCollisionRect().h - offsetY <= floors->GetCollisionRect().y + floors->GetCollisionRect().h)
                {
                    return true;
                }
            }
        }
        return false;
    }


    bool GameCollisionMngr::CheckOverlapWithEnemies(const GameCollisionComponent* box) const
    {
        if (!m_pEnemies.empty())
        {
            for (const auto& otherbox : m_pEnemies)
            {
                if (otherbox == box)
                    continue;

                if (box->GetCollisionRect().x < otherbox->GetCollisionRect().x + otherbox->GetCollisionRect().w &&
                    box->GetCollisionRect().x + box->GetCollisionRect().w > otherbox->GetCollisionRect().x &&
                    box->GetCollisionRect().y < otherbox->GetCollisionRect().y + otherbox->GetCollisionRect().h &&
                    box->GetCollisionRect().y + box->GetCollisionRect().h > otherbox->GetCollisionRect().y)
                {
                    return true;
                }
            }
        }
        return false;
    }

    bool GameCollisionMngr::CheckForOverlapWall(const dae::GameCollisionComponent* box) const
    {
        if (!m_pWallBoxes.empty())
        {
            for (const auto& otherbox : m_pWallBoxes)
            {
                if (box->GetCollisionRect().x < otherbox->GetCollisionRect().x + otherbox->GetCollisionRect().w &&
                    box->GetCollisionRect().x + box->GetCollisionRect().w > otherbox->GetCollisionRect().x &&
                    box->GetCollisionRect().y < otherbox->GetCollisionRect().y + otherbox->GetCollisionRect().h &&
                    box->GetCollisionRect().y + box->GetCollisionRect().h > otherbox->GetCollisionRect().y)
                {
                    return true;
                }
            }
        }
        return false;
    }

    void GameCollisionMngr::PlayerLogicBox(dae::GameCollisionComponent*, glm::vec2)
    {
        //const GameCollisionComponent* OverlappedBox = nullptr;
        //dir = glm::vec2{ 0,0 };
        //if (box)
        //{
        //	OverlappedBox = CheckForCollisionComponent(box);
        //}
    }

    bool dae::GameCollisionMngr::Raycast(glm::vec2 startpos, glm::vec2 direction,const dae::GameCollisionComponent* box) const
    {
        glm::vec2 startPos = startpos;
        startPos.x += box->GetCollisionRect().w / 2.0f;
        startPos.y += box->GetCollisionRect().h / 2.0f;

        glm::vec2 dir = glm::normalize(direction);
        float distance = box->GetCollisionRect().w / 2.f;
        const float offset{ 1.f };
        // Check for collision with obstacles
        for (const auto& boxes : m_pWallBoxes)
        {

            if (startPos.x + (dir.x * distance + offset) <= boxes->GetCollisionRect().x + boxes->GetCollisionRect().w &&
                startPos.x + dir.x * distance - offset >= boxes->GetCollisionRect().x &&
                startPos.y + (dir.y * distance + offset) <= boxes->GetCollisionRect().y + boxes->GetCollisionRect().h &&
                startPos.y + dir.y * distance - offset >= boxes->GetCollisionRect().y)
            {
                return false;
            }

        }
       
        return true;
    }

    bool GameCollisionMngr::AIRaycast(glm::vec2 startpos, glm::vec2 direction, const dae::GameCollisionComponent* box) const
    {
        glm::vec2 startPos = startpos;
        startPos.x += box->GetCollisionRect().w / 2.0f;
        startPos.y += box->GetCollisionRect().h / 2.0f;

        float distance = box->GetCollisionRect().w / 2.f;
        const float offset{ -0.5f };
        // Check for collision with obstacles
        for (const auto& boxes : m_pWallBoxes)
        {

            if (startPos.x + (direction.x * distance + offset) < boxes->GetCollisionRect().x + boxes->GetCollisionRect().w &&
                startPos.x + direction.x * distance - offset > boxes->GetCollisionRect().x &&
                startPos.y + (direction.y * distance + offset) < boxes->GetCollisionRect().y + boxes->GetCollisionRect().h &&
                startPos.y + direction.y * distance - offset > boxes->GetCollisionRect().y)
            {
                return true;
            }
        }
        
        return false;
    }

    bool GameCollisionMngr::AIRaycastUp(glm::vec2 startpos, glm::vec2 direction, const dae::GameCollisionComponent* box) const
    {
        glm::vec2 startPos = startpos;
        startPos.y += box->GetCollisionRect().h / 2.0f;

        float distance = box->GetCollisionRect().w / 2.f;
        const float offset{ 2.f };
        // Check for collision with obstacles
        for (const auto& boxes : m_pWallBoxes)
        {

            if (startPos.y + direction.y * distance - offset >= boxes->GetCollisionRect().y)
            {
                return true;
            }
        }

        return false;
    }

    bool GameCollisionMngr::AIRaycastRight(glm::vec2 startpos, glm::vec2 direction, const dae::GameCollisionComponent* box) const
    {
        glm::vec2 startPos = startpos;
        startPos.x += box->GetCollisionRect().w / 2.0f;

        float distance = box->GetCollisionRect().w / 2.f;
        const float offset{ 2.f };
        // Check for collision with obstacles
        for (const auto& boxes : m_pWallBoxes)
        {
            if (startPos.x + direction.x * distance - offset >= boxes->GetCollisionRect().x)
            {
                return true;
            }
        }

        return false;
    }

    bool GameCollisionMngr::AIRaycastLeft(glm::vec2 startpos, glm::vec2 direction, const dae::GameCollisionComponent* box) const
    {
        glm::vec2 startPos = startpos;
        startPos.x += box->GetCollisionRect().w / 2.0f;
        startPos.y += box->GetCollisionRect().h / 2.0f;

        float distance = box->GetCollisionRect().w / 2.f;
        const float offset{ 2.f };
        // Check for collision with obstacles
        for (const auto& boxes : m_pWallBoxes)
        {

            if (startPos.x + (direction.x * distance + offset) <= boxes->GetCollisionRect().x + boxes->GetCollisionRect().w)
            {
                return true;
            }
        }

        return false;
    }

    bool GameCollisionMngr::AIRaycastDown(glm::vec2 startpos, glm::vec2 direction, const dae::GameCollisionComponent* box) const
    {
        glm::vec2 startPos = startpos;
        startPos.y += box->GetCollisionRect().h / 2.0f;

        float distance = box->GetCollisionRect().w / 2.f;
        const float offset{ 2.f };
        // Check for collision with obstacles
        for (const auto& boxes : m_pWallBoxes)
        {
            if (startPos.y + (direction.y * distance + offset) <= boxes->GetCollisionRect().y + boxes->GetCollisionRect().h)
            {
                return true;
            }
        }

        return false;
    }

}
