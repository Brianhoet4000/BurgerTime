#include "GameCollisionMngr.h"

#include <SDL_syswm.h>

#include "AIMovementComponent.h"
#include "GameObject.h"
#include "IngredientComponent.h"
#include "IngredientPartComponent.h"
#include "PlateComponent.h"
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
            else if (owner->GetTag() == "Ingredient" || owner->GetTag() == "BunTop")
            {
                m_pIngredientBoxes.push_back(box);

                if (owner->GetTag() == "BunTop")

                    m_pBunTopBoxes.push_back(box);
                
            }
            else if (owner->GetTag() == "Egg" || owner->GetTag() == "HotDog" || owner->GetTag() == "Pickle")
            {
                m_pEnemies.push_back(box);
            }
            else if(owner->GetTag() == "Plate")
            {
                m_pPlates.push_back(box);
            }
            else if (owner->GetTag() == "Player_01")
            {
                m_pFirstPlayer = box;
            }
            else if (owner->GetTag() == "Player_02")
            {
                m_pSecondPlayer = box;
            }
            else if(owner->GetTag() == "Bullet")
            {
                m_pBullet.push_back(box);
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
        m_pBullet.erase(std::remove(m_pBullet.begin(), m_pBullet.end(), box), m_pBullet.end());
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
        m_pIngredientBoxes.clear();
        m_pBunTopBoxes.clear();
        m_pEnemies.clear();
        m_pPlates.clear();
        //m_pFirstPlayer = nullptr;
        //m_pSecondPlayer = nullptr;
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
            if (box->GetOwner()->GetComponent<AIMovementComponent>()->ReturnStunned())
            {
                return nullptr;
            }

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

    void GameCollisionMngr::CheckIngredientOverlapWithEnemies(const GameCollisionComponent* box) const
    {
        for (const auto& EnemyBox : m_pEnemies)
        {
            if (box->GetCollisionRect().x < EnemyBox->GetCollisionRect().x + EnemyBox->GetCollisionRect().w &&
                box->GetCollisionRect().x + box->GetCollisionRect().w > EnemyBox->GetCollisionRect().x &&
                box->GetCollisionRect().y < EnemyBox->GetCollisionRect().y + EnemyBox->GetCollisionRect().h &&
                box->GetCollisionRect().y + box->GetCollisionRect().h > EnemyBox->GetCollisionRect().y)
            {
                if (box->GetOwner()->GetParent()->GetChildren()[0])
                {
                    const auto& player = box->GetOwner()->GetParent()->GetChildren()[0]->GetComponent<IngredientPartComponent>()->GetPlayer();
                    dae::ScreenManager::GetInstance().IncreasePoint(player, 500);
                    std::cout << "1\n";
                }

                EnemyBox->GetOwner()->MarkTrueForDeleting();
            }
        }
    }

    GameCollisionComponent* GameCollisionMngr::GetCurrentFloor(const GameCollisionComponent* box) const
    {
        for (const auto& floor : m_pFloorBoxes)
        {
            if (box->GetCollisionRect().x < floor->GetCollisionRect().x + floor->GetCollisionRect().w &&
                box->GetCollisionRect().x + box->GetCollisionRect().w > floor->GetCollisionRect().x &&
                box->GetCollisionRect().y < floor->GetCollisionRect().y + floor->GetCollisionRect().h &&
                box->GetCollisionRect().y + box->GetCollisionRect().h > floor->GetCollisionRect().y)
            {
                return floor;
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

    bool GameCollisionMngr::CheckOverlapWithPlayersBoolIngredients(const GameCollisionComponent* box) const
    {
        for (const auto& player : PlayerManager::GetInstance().GetPlayers())
        {
            if (player == nullptr) return false;
            if (player->ReturnDeleting()) return false;

            const auto& PlayerBox = player->GetComponent<GameCollisionComponent>();

            if (PlayerBox->GetIsVersus()) return false;

            int offsetX = 10; // Offset towards the center
            int offsetY = 0; // Offset towards the center

            if (box->GetCollisionRect().x < PlayerBox->GetCollisionRect().x + offsetX &&
                box->GetCollisionRect().x + box->GetCollisionRect().w > PlayerBox->GetCollisionRect().x + offsetX &&
                box->GetCollisionRect().y < PlayerBox->GetCollisionRect().y + offsetY &&
                box->GetCollisionRect().y + box->GetCollisionRect().h > PlayerBox->GetCollisionRect().y + offsetY)
            {
                dae::servicelocator::get_sound_system().playSound(3, 50);
                box->GetOwner()->GetComponent<IngredientPartComponent>()->SetPlayer(PlayerBox->GetOwner());
                return true;
            }
        }
        box->GetOwner()->GetComponent<IngredientPartComponent>()->SetPlayer(PlayerManager::GetInstance().GetPlayers()[0].get());
        return false;

    }

    bool GameCollisionMngr::CheckOverlapIngredientsWithFloors(const GameCollisionComponent* box) const
    {
        for (const auto& floor : m_pFloorBoxes)
        {

            if (box->GetCollisionRect().x < floor->GetCollisionRect().x + floor->GetCollisionRect().w &&
                box->GetCollisionRect().x + box->GetCollisionRect().w > floor->GetCollisionRect().x &&
                box->GetOwner()->GetRelativePosition().y < floor->GetCollisionRect().y + floor->GetCollisionRect().h &&
                box->GetOwner()->GetRelativePosition().y + box->GetCollisionRect().h - 10 > floor->GetCollisionRect().y)
            {
                return true;
            }
        }
        return false;
    }

    GameCollisionComponent* GameCollisionMngr::CheckOverlapIngredientsForCurrentFloor(const GameCollisionComponent* box) const
    {
        for (const auto& floor : m_pFloorBoxes)
        {
            if (box->GetCollisionRect().x < floor->GetCollisionRect().x + floor->GetCollisionRect().w &&
                box->GetCollisionRect().x + box->GetCollisionRect().w > floor->GetCollisionRect().x &&
                box->GetCollisionRect().y < floor->GetCollisionRect().y + floor->GetCollisionRect().h &&
                box->GetCollisionRect().y + box->GetCollisionRect().h > floor->GetCollisionRect().y)
            {
                return floor;
            }
        }
        return nullptr;
    }


    void GameCollisionMngr::CheckOverlapIngredientsWithOtherIngredients(const GameCollisionComponent* box) const
    {
        for (const auto& OtherIngredients : m_pIngredientBoxes)
        {
            if (box->GetCollisionRect().x < OtherIngredients->GetCollisionRect().x + OtherIngredients->GetCollisionRect().w &&
                box->GetCollisionRect().x + box->GetCollisionRect().w > OtherIngredients->GetCollisionRect().x &&
                box->GetCollisionRect().y < OtherIngredients->GetCollisionRect().y + OtherIngredients->GetCollisionRect().h &&
                box->GetCollisionRect().y + box->GetCollisionRect().h > OtherIngredients->GetCollisionRect().y)
                {
					OtherIngredients->GetOwner()->GetComponent<IngredientPartComponent>()->SetCollided(true);
                    OtherIngredients->GetOwner()->GetComponent<IngredientPartComponent>()->
            		SetPlayer(box->GetOwner()->GetComponent<IngredientPartComponent>()->GetPlayer());
                }
        }
    }

    void GameCollisionMngr::CheckOverlapIngredientsWithPlates(const GameCollisionComponent* box, int amountOfIngredient) const
    {
        for (const auto& ingredient : m_pIngredientBoxes)
        {
            if(ingredient->GetOwner()->GetParent()->GetComponent<IngredientComponent>()->GetIsCompleted())
                continue;

			float YDepending = 0.0f;

            switch (amountOfIngredient)
            {
            case 0:
                YDepending = 21.0f * 3;
                break;
            case 1:
                YDepending = 21.0f * 2;
                break;
            case 2:
                YDepending = 21.0f * 1;
                break;
            case 3:
                YDepending = 0.0f;
                break;
            default:
                YDepending = 0.0f;
                break;
			}
        
            if (box->GetCollisionRect().x < ingredient->GetCollisionRect().x &&
                box->GetCollisionRect().x + box->GetCollisionRect().w > ingredient->GetCollisionRect().x + ingredient->GetCollisionRect().w &&
                box->GetCollisionRect().y + YDepending < ingredient->GetCollisionRect().y &&
                box->GetCollisionRect().y + box->GetCollisionRect().h + YDepending > ingredient->GetCollisionRect().y + ingredient->GetCollisionRect().h)
            {
                ingredient->GetOwner()->GetParent()->GetComponent<IngredientComponent>()->SetIscompleted(true);
                box->GetOwner()->GetComponent<PlateComponent>()->IncrementInt();
            }
        }
    }

    bool GameCollisionMngr::CheckOverlapTopBunWithPlates(const GameCollisionComponent* box) const
    {
        for (const auto& Topbun : m_pBunTopBoxes)
        {
            if (box->GetCollisionRect().x < Topbun->GetCollisionRect().x &&
                box->GetCollisionRect().x + box->GetCollisionRect().w > Topbun->GetCollisionRect().x + Topbun->GetCollisionRect().w &&
                box->GetCollisionRect().y < Topbun->GetCollisionRect().y &&
                box->GetCollisionRect().y + box->GetCollisionRect().h > Topbun->GetCollisionRect().y + Topbun->GetCollisionRect().h)
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

    bool GameCollisionMngr::CheckOverlapWithStairs(const GameCollisionComponent* playerBox) const
    {

        for (const auto& stairs : m_pStairsBoxes)
        {
            if (stairs == nullptr)
                return false;

            if (playerBox->GetCollisionRect().x >= stairs->GetCollisionRect().x &&
                playerBox->GetCollisionRect().x + playerBox->GetCollisionRect().w <= stairs->GetCollisionRect().x + stairs->GetCollisionRect().w &&
                playerBox->GetCollisionRect().y >= stairs->GetCollisionRect().y &&
                playerBox->GetCollisionRect().y + playerBox->GetCollisionRect().h <= stairs->GetCollisionRect().y + stairs->GetCollisionRect().h)
            {
                return true;
            }
        }
        return false;
    }

    bool GameCollisionMngr::MoveOutsideColliderUpStairs(const GameCollisionComponent* playerBox) const
    {
        for (const auto& stairs : m_pStairsBoxes)
        {
            if (stairs == nullptr)
                return false;

            if (playerBox->GetCollisionRect().x >= stairs->GetCollisionRect().x &&
                playerBox->GetCollisionRect().x + playerBox->GetCollisionRect().w <= stairs->GetCollisionRect().x + stairs->GetCollisionRect().w &&
                playerBox->GetCollisionRect().y < stairs->GetCollisionRect().y && playerBox->GetCollisionRect().y > stairs->GetCollisionRect().y - 10) // Going up
            {
                return true;
            }
        }
        return false;
    }


    bool GameCollisionMngr::MoveOutsideColliderDownStairs(const GameCollisionComponent* playerBox) const
    {
        for (const auto& stairs : m_pStairsBoxes)
        {
            if (stairs == nullptr)
                return false;

            if (playerBox->GetCollisionRect().x >= stairs->GetCollisionRect().x &&
                playerBox->GetCollisionRect().x + playerBox->GetCollisionRect().w <= stairs->GetCollisionRect().x + stairs->GetCollisionRect().w &&
                playerBox->GetCollisionRect().y + playerBox->GetCollisionRect().h > stairs->GetCollisionRect().y + stairs->GetCollisionRect().h &&
                playerBox->GetCollisionRect().y + playerBox->GetCollisionRect().h < stairs->GetCollisionRect().y + stairs->GetCollisionRect().h + 10)
            {
                return true;
            }
        }
        return false;
    }

    bool GameCollisionMngr::CheckOverlapWithFloors(const GameCollisionComponent* playerBox) const
    {
        constexpr int offsetX = 2;
        constexpr int offsetY = 2;

        for (const auto& floors : m_pFloorBoxes)
        {
            if (floors == nullptr)
                return false;
            
            if (playerBox->GetCollisionRect().x + offsetX >= floors->GetCollisionRect().x &&
                playerBox->GetCollisionRect().x + playerBox->GetCollisionRect().w - offsetX <= floors->GetCollisionRect().x + floors->GetCollisionRect().w &&
                playerBox->GetCollisionRect().y + offsetY >= floors->GetCollisionRect().y &&
                playerBox->GetCollisionRect().y + playerBox->GetCollisionRect().h - offsetY <= floors->GetCollisionRect().y + floors->GetCollisionRect().h)
            {
                return true;
            }
        }
        return false;
    }

    bool GameCollisionMngr::MoveOutsideColliderLeftStairs(const GameCollisionComponent* playerBox) const
    {
        const float heightOffset = 5.f;

        for (const auto& floors : m_pFloorBoxes)
        {
            if (floors == nullptr)
                return false;

            if (playerBox->GetCollisionRect().y >= floors->GetCollisionRect().y - heightOffset &&
                playerBox->GetCollisionRect().y + playerBox->GetCollisionRect().h <= floors->GetCollisionRect().y + floors->GetCollisionRect().h + heightOffset &&
                playerBox->GetCollisionRect().x <= floors->GetCollisionRect().x &&
                playerBox->GetCollisionRect().x + playerBox->GetCollisionRect().w >= floors->GetCollisionRect().x - 10)
            {
                return true;
            }
        }
        return false;
    }


    bool GameCollisionMngr::MoveOutsideColliderRightStairs(const GameCollisionComponent* playerBox) const
    {
        const float heightOffset = 5.f;

        for (const auto& floors : m_pFloorBoxes)
        {
            if (floors == nullptr)
                return false;

            if (playerBox->GetCollisionRect().y >= floors->GetCollisionRect().y - heightOffset &&
                playerBox->GetCollisionRect().y + playerBox->GetCollisionRect().h <= floors->GetCollisionRect().y + floors->GetCollisionRect().h + heightOffset &&
                playerBox->GetCollisionRect().x + playerBox->GetCollisionRect().w >= floors->GetCollisionRect().x + floors->GetCollisionRect().w
                && playerBox->GetCollisionRect().x <= floors->GetCollisionRect().x + floors->GetCollisionRect().w + 10)
            {
                return true;
            }
        }
        return false;
    }

    bool GameCollisionMngr::CheckOverlapWithIngredient(const GameCollisionComponent*) const
    {
        if (!m_pIngredientBoxes.empty())
        {
            for (const auto& player : PlayerManager::GetInstance().GetPlayers())
            {
                for (const auto& otherbox : m_pIngredientBoxes)
                {
                    const auto& box = player->GetComponent<GameCollisionComponent>();

                    float offsetX = 16; // Offset towards the center
                    float offsetY = 0; // Offset towards the center

                    if ((otherbox->GetCollisionRect().x <= box->GetCollisionRect().x + offsetX &&
                        otherbox->GetCollisionRect().x + otherbox->GetCollisionRect().w >= box->GetCollisionRect().x + offsetX &&
                        otherbox->GetCollisionRect().y <= box->GetCollisionRect().y + offsetY &&
                        otherbox->GetCollisionRect().y + otherbox->GetCollisionRect().h >= box->GetCollisionRect().y + offsetY))
                    {
                        std::cout << m_pIngredientBoxes.size() << '\n';
                        std::cout << otherbox->GetOwner()->GetRelativePosition().x << ", " << otherbox->GetOwner()->GetRelativePosition().y << '\n';
                        //otherbox->GetOwner()->GetComponent<IngredientPartComponent>()->SetPushedDown(true);
                        return true;
                    }
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
