#include "GameCommands.h"

#include "AIMovementComponent.h"
#include "CollisionBoxManager.h"
#include "GameCollisionMngr.h"
#include "InputManager.h"
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
    }

    const float offset{ 3.5f };
    if (m_Dir.x > 0.2f || m_Dir.x < -0.2f)
    {
        if(dae::GameCollisionMngr::GetInstance().MovePlayerLeftFloors(m_pGameObject->GetComponent<dae::GameCollisionComponent>()))
        {
            pos.x = m_pGameObject->GetRelativePosition().x + offset;
            m_pGameObject->SetRelativePosition(pos);
            return;
        }
        else if(dae::GameCollisionMngr::GetInstance().MovePlayerRightFloors(m_pGameObject->GetComponent<dae::GameCollisionComponent>()))
        {
            pos.x = m_pGameObject->GetRelativePosition().x - offset;
            m_pGameObject->SetRelativePosition(pos);
            return;
        }
        else if (dae::GameCollisionMngr::GetInstance().CheckOverlapWithFloors(m_pGameObject->GetComponent<dae::GameCollisionComponent>()))
        {
            pos.x += m_Dir.x * deltaTime;
            m_pGameObject->SetRelativePosition(pos);
            return;
        }
        return;
    }

    if (m_Dir.y > 0.2f || m_Dir.y < -0.2f)
    {
        if (dae::GameCollisionMngr::GetInstance().MovePlayerDownStairs(m_pGameObject->GetComponent<dae::GameCollisionComponent>()))
        {
            pos.y = m_pGameObject->GetRelativePosition().y - offset;
            m_pGameObject->SetRelativePosition(pos);
            return;
        }
        else if (dae::GameCollisionMngr::GetInstance().MovePlayerUpStairs(m_pGameObject->GetComponent<dae::GameCollisionComponent>()))
        {
            pos.y = m_pGameObject->GetRelativePosition().y + offset;
            m_pGameObject->SetRelativePosition(pos);
            return;
        }
        else if (dae::GameCollisionMngr::GetInstance().CheckOverlapWithStairs(m_pGameObject->GetComponent<dae::GameCollisionComponent>()))
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
    if (m_pGameObject->ReturnDeleting()) return;
    if (m_pBulletTimer->ReturnHasShot()) return;

    auto shootingState = m_pGameObject->GetComponent<dae::ShootingDirComponent>();

    if (!shootingState) return;

    if (shootingState->returnFaceState() == dae::ShootingDirComponent::Right)
    {
        m_Dir = { 1,0 };
        std::cout << "stunned right\n";
    }
    if (shootingState->returnFaceState() == dae::ShootingDirComponent::Left)
    {
        m_Dir = { -1,0 };
        std::cout << "stunned left\n";
    }

    dae::SceneManager* Scenemanager;
    for (auto element : Scenemanager->GetInstance().GetActiveScene()->GetGameObjects())
    {
		if(element->GetTag() == "Enemy")
		{
            std::cout << "Found\n";
            element->GetComponent<dae::AIMovementComponent>()->Stunned();
		}
    }

    m_pBulletTimer->SetHasShot(true);
    SetKeyPressed(true);
}

GameCommands::SwitchGameMode::SwitchGameMode(std::shared_ptr<dae::GameObject> owner, dae::GameObject* text, dae::ScreenManager::GameMode& currentScreen, dae::ScreenManager* screen)
	:m_pScreen{std::move(owner)},
    m_pTextMode{text},
    m_CurrentScreen{ currentScreen },
	m_pScreenManager{ screen }
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
    
    dae::GameCollisionMngr::GetInstance().ClearAll();
    dae::SceneManager::GetInstance().GetActiveScene()->RemoveAll();
    dae::ScreenManager::GetInstance().CreateGameScreen(*dae::SceneManager::GetInstance().GetActiveScene());
}

void GameCommands::MuteMusic::Execute(float)
{
    if (GetKeyPressed()) return;

    dae::servicelocator::get_sound_system().MuteUnmute();

    SetKeyPressed(true);
}
