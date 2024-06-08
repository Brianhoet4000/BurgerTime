#include "GameCommands.h"

#include "AIMovementComponent.h"
#include "BulletPrefab.h"
#include "CollisionBoxManager.h"
#include "GameCollisionMngr.h"
#include "HealthComponent.h"
#include "InputManager.h"
#include "PlayerManager.h"
#include "PointComponent.h"
#include "ServiceLocator.h"
#include "ShootingDirComponent.h"

GameCommands::PlayerMovement::PlayerMovement(std::shared_ptr<dae::GameObject> owner, const glm::vec2& dir, bool player)
{
	m_pGameObject = std::move(owner);
	m_Dir = dir;
    m_pCollision = m_pGameObject->GetComponent<dae::GameCollisionComponent>();
    m_Player = player;
}

void GameCommands::PlayerMovement::Execute(float deltaTime)
{
    if (m_pGameObject->ReturnDeleting()) return;
    glm::vec2 pos = m_pGameObject->GetRelativePosition();

    if (m_Player)
    {
        if (m_pGameObject->GetComponent<dae::BulletTimerComponent>()->ReturnHasShot()) return;

        const auto& shootingstate = m_pGameObject->GetComponent<dae::ShootingDirComponent>();
        if (shootingstate == nullptr) return;
        if (m_Dir.x > 0)
        {
            shootingstate->SetFaceState(dae::ShootingDirComponent::Right);
        }
        if (m_Dir.x < 0)
        {
            shootingstate->SetFaceState(dae::ShootingDirComponent::Left);
        }


        if (dae::GameCollisionMngr::GetInstance().CheckOverlapWithSecondPlayerVersus(m_pCollision))
        {
            dae::ScreenManager::GetInstance().PlayerKilledResetLevelAndStats(m_pCollision);
            return;
        }
    }
    else
    {
        if(m_pGameObject->GetComponent<dae::BulletTimerComponent>()->ReturnCompleted())
        {
            m_pGameObject->GetComponent<dae::BulletTimerComponent>()->ResetComplete();
        }

        if (m_pGameObject->GetComponent<dae::BulletTimerComponent>()->ReturnHasShot())
            return;

       

        const auto& firstPlayer = dae::GameCollisionMngr::GetInstance().CheckOverlapWithFirstPlayer(m_pGameObject->GetComponent<dae::GameCollisionComponent>());
        if (firstPlayer)
        {
            dae::ScreenManager::GetInstance().PlayerKilledResetLevelAndStats(firstPlayer);
            return;
        }
    }

    const float offset{ 2.0f };
    if (m_Dir.x > 0.2f || m_Dir.x < -0.2f)
    {
        if(dae::GameCollisionMngr::GetInstance().MoveOutsideColliderLeftStairs(m_pCollision))
        {
            pos.x = m_pGameObject->GetRelativePosition().x + offset;
            m_pGameObject->SetRelativePosition(pos);
            return;
        }
        else if(dae::GameCollisionMngr::GetInstance().MoveOutsideColliderRightStairs(m_pCollision))
        {
            pos.x = m_pGameObject->GetRelativePosition().x - offset;
            m_pGameObject->SetRelativePosition(pos);
            return;
        }
        else if (dae::GameCollisionMngr::GetInstance().CheckOverlapWithFloors(m_pCollision))
        {
            pos.x += m_Dir.x * deltaTime;
            m_pGameObject->SetRelativePosition(pos);
            return;
        }
        return;
    }

    if (m_Dir.y > 0.2f || m_Dir.y < -0.2f)
    {
        if (dae::GameCollisionMngr::GetInstance().MoveOutsideColliderDownStairs(m_pCollision))
        {
            pos.y = m_pGameObject->GetRelativePosition().y - offset;
            m_pGameObject->SetRelativePosition(pos);
            return;
        }
        else if (dae::GameCollisionMngr::GetInstance().MoveOutsideColliderUpStairs(m_pCollision))
        {
            pos.y = m_pGameObject->GetRelativePosition().y + offset;
            m_pGameObject->SetRelativePosition(pos);
            return;
        }
        else if (dae::GameCollisionMngr::GetInstance().CheckOverlapWithStairs(m_pCollision))
        {
            pos.y += m_Dir.y * deltaTime;
            m_pGameObject->SetRelativePosition(pos);
            return;
        }
    }
}

GameCommands::Stun::Stun(std::shared_ptr<dae::GameObject> owner)
{
    m_pGameObject = owner;
    m_pBulletTimer = owner->GetComponent<dae::BulletTimerComponent>();
}

void GameCommands::Stun::Execute(float)
{
    if (GetKeyPressed()) return;
    if (m_pGameObject == nullptr) return;
    if (m_pGameObject->GetComponent<dae::CounterComponent>()->GetAmount() <= 0) return;
    if (m_pGameObject->ReturnDeleting()) return;
    if (m_pBulletTimer->ReturnHasShot()) return;

    const auto& shootingState = m_pGameObject->GetComponent<dae::ShootingDirComponent>();
    if (!shootingState) return;

    std::unique_ptr<dae::BulletPrefab> bullet = std::make_unique<dae::BulletPrefab>(*dae::SceneManager::GetInstance().GetActiveScene(),
        m_pGameObject->GetRelativePosition(), shootingState->returnFaceState());

    m_pGameObject->GetComponent<dae::CounterComponent>()->DecreaseAmount(1);
    dae::ScreenManager::GetInstance().DecreaseSpray(m_pGameObject.get());

    bullet->returnGameObject()->GetComponent<dae::BulletTimerComponent>()->SetHasShot(true);
    m_pBulletTimer->SetHasShot(true);
    SetKeyPressed(true);
}

GameCommands::SwitchGameMode::SwitchGameMode(std::shared_ptr<dae::GameObject> owner, dae::GameObject* text, dae::ScreenManager::GameMode& currentScreen, dae::ScreenManager* screen)
	:m_pScreen{std::move(owner)},
    m_pTextMode{text},
	m_pScreenManager{ screen },
    m_CurrentScreen{ currentScreen }
{
}

void GameCommands::SwitchGameMode::Execute(float)
{
    if (GetKeyPressed()) return;

    auto text = m_pTextMode->GetComponent<dae::TextComponent>();

    switch (m_CurrentScreen)
    {
    case 0:
        m_CurrentScreen = dae::ScreenManager::GameMode::Coop;
        text->SetText("Coop");
        break;

    case 1:
        m_CurrentScreen = dae::ScreenManager::GameMode::Versus;
        text->SetText("Versus");
        break;

    case 2:
        m_CurrentScreen = dae::ScreenManager::GameMode::SinglePlayer;
        text->SetText("Single");
        break;
    }

    m_pScreenManager->SetCurrentEnum(m_CurrentScreen);

    SetKeyPressed(true);
}

void GameCommands::AcceptGameMode::Execute(float)
{
    if (GetKeyPressed()) return;

    if (dae::SceneManager::GetInstance().GetActiveSceneName() == "MainMenu")
    {
        dae::SceneManager::GetInstance().NextScene();
        dae::ScreenManager::GetInstance().CreateGameScreen(*dae::SceneManager::GetInstance().GetActiveScene());
        dae::servicelocator::get_sound_system().playMusic(0, 10);
        dae::servicelocator::get_sound_system().playSound(5, 20);
        SetKeyPressed(true);
    }
}

void GameCommands::SkipLevel::Execute(float)
{
    if (GetKeyPressed()) return;

    if(dae::SceneManager::GetInstance().GetActiveSceneName() == "GameOver" ||
        dae::SceneManager::GetInstance().GetActiveSceneName() == "MainMenu")
    {
        SetKeyPressed(true);
        return;
    }

    dae::ScreenManager::GetInstance().ProceedNextLevel();

    SetKeyPressed(true);
}

void GameCommands::ResetLevel::Execute(float)
{
    if (GetKeyPressed()) return;

    if (dae::SceneManager::GetInstance().GetActiveSceneName() == "GameOver" ||
        dae::SceneManager::GetInstance().GetActiveSceneName() == "MainMenu")
    {
        SetKeyPressed(true);
        return;
    }

    for (const auto& player : dae::PlayerManager::GetInstance().GetPlayers())
    {
        player->GetComponent<dae::PointComponent>()->SetAmount(0);
        player->GetComponent<dae::HealthComponent>()->SetAmount(3);
    }
    
    dae::GameCollisionMngr::GetInstance().ClearAll();
    dae::SceneManager::GetInstance().GetActiveScene()->RemoveAll();
    dae::ScreenManager::GetInstance().CreateGameScreen(*dae::SceneManager::GetInstance().GetActiveScene());

    for (const auto& player : dae::PlayerManager::GetInstance().GetPlayers())
    {
        player->SetUpdate(true);
    }
}

void GameCommands::MuteMusic::Execute(float)
{
    if (GetKeyPressed()) return;

    dae::servicelocator::get_sound_system().MuteUnmute();

    SetKeyPressed(true);
}
