#include "PlayerOne.h"
#include <SDL_scancode.h>
#include "Commands.h"
#include "InputManager.h"
#include <TextureComponent.h>
#include "Counter.h"
#include "GameCommands.h"
#include "HealthComponent.h"
#include "PointComponent.h"
#include "ResourceManager.h"
#include "ShootingDirComponent.h"

dae::PlayerOne::PlayerOne(dae::Scene& scene, dae::LevelPrefab&)
{
	m_pPlayerOne = std::make_shared<dae::GameObject>("Player_01");

	//Texture
	const auto& pTexture = std::make_shared<dae::TextureComponent>(m_pPlayerOne.get());
	pTexture->SetTexture("Characters/PeterPepper.png");
	m_pPlayerOne->AddComponent(pTexture);

	//Collision
	//const auto& pCollider = std::make_shared<dae::GameCollisionComponent>(m_pPlayerOne.get(), m_pPlayerOne->GetRelativePosition(), pTexture->GetSize().x,
	//	pTexture->GetSize().y, true, true, false);
	//m_pPlayerOne->AddComponent(pCollider);
	//pCollider->SetCollisionRectOffset(2.f);
	//pCollider->SetDebugColor("green");
	//pCollider->SetRenderCollisionBox(true);

	const auto& pFootCollider = std::make_shared<dae::GameCollisionComponent>(m_pPlayerOne.get(), m_pPlayerOne->GetRelativePosition(),
		pTexture->GetSize().x, 4.f, true, true, false); // Use positive height
	m_pPlayerOne->AddComponent(pFootCollider);
	pFootCollider->SetCollisionRectOffset(3.f, pTexture->GetSize().y - 4.f, true); // Position below the texture
	pFootCollider->SetDebugColor("red");
	pFootCollider->SetRenderCollisionBox(true);



	//BulletTimer
	const auto& pTimer = std::make_shared<dae::BulletTimerComponent>(m_pPlayerOne.get());
	m_pPlayerOne->AddComponent(pTimer);

	//ShootingDir
	const auto& pShootingDir = std::make_shared<ShootingDirComponent>(m_pPlayerOne.get());
	m_pPlayerOne->AddComponent(pShootingDir);

	//Movement
	std::shared_ptr<GameCommands::DiggerMovement> moveCommandUp = std::make_shared<GameCommands::DiggerMovement>(m_pPlayerOne, m_Up);
	std::shared_ptr<GameCommands::DiggerMovement> moveCommandDown = std::make_shared<GameCommands::DiggerMovement>(m_pPlayerOne, m_Down);
	std::shared_ptr<GameCommands::DiggerMovement> moveCommandLeft = std::make_shared<GameCommands::DiggerMovement>(m_pPlayerOne, m_Left);
	std::shared_ptr<GameCommands::DiggerMovement> moveCommandRight = std::make_shared<GameCommands::DiggerMovement>(m_pPlayerOne, m_Right);

	dae::InputManager::GetInstance().BindKeyToCommand(SDL_SCANCODE_W, moveCommandUp);
	dae::InputManager::GetInstance().BindKeyToCommand(SDL_SCANCODE_S, moveCommandDown);
	dae::InputManager::GetInstance().BindKeyToCommand(SDL_SCANCODE_A, moveCommandLeft);
	dae::InputManager::GetInstance().BindKeyToCommand(SDL_SCANCODE_D, moveCommandRight);

	if (dae::ScreenManager::GetInstance().GetCurrentEnum() != dae::ScreenManager::SinglePlayer)
		m_Controller1Index = 1;
	else m_Controller1Index = 0;
	
	dae::InputManager::GetInstance().AddController(m_Controller1Index);

	dae::Controller::ControllerButton controllerButton{};

	controllerButton = dae::Controller::ControllerButton::DpadUp;
	dae::InputManager::GetInstance().BindControllerToCommand(m_Controller1Index, controllerButton, moveCommandUp);
	controllerButton = dae::Controller::ControllerButton::DpadDown;
	dae::InputManager::GetInstance().BindControllerToCommand(m_Controller1Index, controllerButton, moveCommandDown);
	controllerButton = dae::Controller::ControllerButton::DpadLeft;
	dae::InputManager::GetInstance().BindControllerToCommand(m_Controller1Index, controllerButton, moveCommandLeft);
	controllerButton = dae::Controller::ControllerButton::DpadRight;
	dae::InputManager::GetInstance().BindControllerToCommand(m_Controller1Index, controllerButton, moveCommandRight);
	

	const auto& pHealth = std::make_shared<dae::HealthComponent>(m_pPlayerOne.get(), 3);
	m_pPlayerOne->AddComponent(pHealth);

	const auto& pPoints = std::make_shared<dae::PointComponent>(m_pPlayerOne.get(), 0);
	m_pPlayerOne->AddComponent(pPoints);

	scene.Add(m_pPlayerOne);
}
