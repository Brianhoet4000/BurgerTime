#include "EnemyStates.h"
#include <random>
#include "GameCollisionMngr.h"
#include "PlayerManager.h"

EnemyMovement::EnemyMovement(dae::GameObject* owner, glm::vec2)
	:m_pOwner(owner)
{
	m_Dir = m_DirRight;
	m_OnStairs = dae::GameCollisionMngr::GetInstance().CheckOverlapWithStairs(m_pOwner->GetComponent<dae::GameCollisionComponent>());
	m_OnFloor = dae::GameCollisionMngr::GetInstance().CheckOverlapWithFloors(m_pOwner->GetComponent<dae::GameCollisionComponent>());
}

void EnemyMovement::OnEnter()
{
	m_update = true;
}

void EnemyMovement::Update(float deltaTime)
{
    if (!m_update) return;

    // Step 1: Determine the closest player
    GetClosestPlayer();

    if (m_pClosestPlayer == nullptr) return;

    // Step 2: Calculate the direction towards the closest player
    glm::vec2 playerPos = m_pClosestPlayer->GetRelativePosition();
    glm::vec2 enemyPos = m_pOwner->GetRelativePosition();
    glm::vec2 directionToPlayer = glm::normalize(playerPos - enemyPos);

    // Step 3: Check for overlap with stairs and floors
    const auto collider = m_pOwner->GetComponent<dae::GameCollisionComponent>();
    const auto& gameCollMngr = dae::GameCollisionMngr::GetInstance();

    m_OnStairs = gameCollMngr.CheckOverlapWithStairs(collider);
    m_OnFloor = gameCollMngr.CheckOverlapWithFloors(collider);

    // Step 4: Adjust direction based on overlap and tag
    if (m_pOwner->GetTag() == "HotDog" || m_pOwner->GetTag() == "Egg")
    {
        // If on stairs
        if (m_OnStairs)
        {
            // Check if the player is above or below the enemy
            if (playerPos.y - 0.5f < enemyPos.y)
            {
                m_Dir = m_DirUp; // Move up to align with player's position
            }
            else if (playerPos.y + 0.5f > enemyPos.y)
            {
                m_Dir = m_DirDown; // Move down to align with player's position
            }
        }
        // If on floor
        else if (m_OnFloor)
        {
            // Check if the player is to the left or right of the enemy
            if (playerPos.x - 0.5f < enemyPos.x)
            {
                m_Dir = m_DirLeft; // Move left towards the player
            }
            else if (playerPos.x + 0.5f > enemyPos.x)
            {
                m_Dir = m_DirRight; // Move right towards the player
            }
        }

        // Step 5: Move AI based on the determined direction
        MoveAI(deltaTime, m_Dir);
    }
    else if (m_pOwner->GetTag() == "Pickle")
    {
        if (m_OnStairs)
        {
            // Check if the player is above or below the enemy
            if (playerPos.y + 0.5f < enemyPos.y)
            {
                m_Dir = m_DirUp; // Move up to align with player's position
            }
            else if (playerPos.y - 0.5f > enemyPos.y)
            {
                m_Dir = m_DirDown; // Move down to align with player's position
            }
        }
        else if (m_OnFloor)
        {
            // Check if the player is to the left or right of the enemy
            if (playerPos.x - 0.5f < enemyPos.x)
            {
                m_Dir = m_DirLeft;
            }
            else if (playerPos.x + 0.5f > enemyPos.x)
            {
                m_Dir = m_DirRight;
            }
        }

        // Step 5: Move AI based on the determined direction
        MoveAI(deltaTime, m_Dir);
    }
}

void EnemyMovement::GetClosestPlayer()
{
	const auto& pPlayers = dae::PlayerManager::GetInstance().GetPlayersNotVersus();

	if (pPlayers.empty()) return;

	float closestdist{ INFINITY };

	for (const auto& player : pPlayers)
	{
		if (player == nullptr) continue;

		const float dist = glm::distance(player->GetRelativePosition(), m_pOwner->GetRelativePosition());
		if (dist < closestdist)
		{
			closestdist = dist;
			m_pClosestPlayer = player.get();
		}
	}
}

void EnemyMovement::OnExit()
{
	m_update = false;
}

EnemyStunned::EnemyStunned(dae::GameObject* owner, dae::BulletTimerComponent* timer)
	:m_pOwner(owner)
{
	m_pStunTimer = timer;
}

void EnemyStunned::OnEnter()
{
	m_update = true;
	m_pStunTimer->SetHasShot(true);
}

void EnemyStunned::Update(float deltaTime)
{
	if (!m_update) return;

	m_pStunTimer->Update(deltaTime);

	if (m_pStunTimer->ReturnHasShot()) return;

	if (m_pStunTimer->ReturnCompleted())
	{
		m_update = false;
	}
}

void EnemyStunned::OnExit()
{
	m_update = false;
	m_pStunTimer->ReturnCompleted();
}
