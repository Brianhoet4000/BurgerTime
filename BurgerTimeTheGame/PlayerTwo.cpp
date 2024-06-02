#include "PlayerTwo.h"
#include <algorithm>
#include "Commands.h"
#include "InputManager.h"
#include <TextureComponent.h>
#include "Counter.h"
#include "EnemyStates.h"
#include "GameCommands.h"
#include "HealthComponent.h"
#include "PointComponent.h"
#include "ResourceManager.h"
#include "ShootingDirComponent.h"

dae::PlayerTwo::PlayerTwo(dae::Scene& scene, dae::LevelPrefab&, bool Coop)
{
	m_pPlayerTwo = std::make_shared<dae::GameObject>("Player_02");

	std::shared_ptr<GameCommands::PlayerMovement> moveCommandUp;
	std::shared_ptr<GameCommands::PlayerMovement> moveCommandDown;
	std::shared_ptr<GameCommands::PlayerMovement> moveCommandLeft;
	std::shared_ptr<GameCommands::PlayerMovement> moveCommandRight;

	const int controller1Index{ 0 };
	dae::InputManager::GetInstance().AddController(controller1Index);

	dae::Controller::ControllerButton controllerButton{};

	if (Coop)
	{
		//Texture Coop
		const auto& pTexture = std::make_shared<dae::TextureComponent>(m_pPlayerTwo.get());
		pTexture->SetTexture("Characters/SallySalt.png");
		m_pPlayerTwo->AddComponent(pTexture);

		//Collision
		//const auto& pCollider = std::make_shared<dae::GameCollisionComponent>(m_pPlayerTwo.get(), level.GetSpawnPosition()[1], pTexture->GetSize().x, pTexture->GetSize().y, true, false, false);
		//pCollider->SetCollisionRectOffset(5.f);
		//pCollider->SetRenderCollisionBox(true);
		//m_pPlayerTwo->AddComponent(pCollider);

		const auto& pFootCollider = std::make_shared<dae::GameCollisionComponent>(m_pPlayerTwo.get(), m_pPlayerTwo->GetRelativePosition(),
			pTexture->GetSize().x, 8.f, true, true, false);
		m_pPlayerTwo->AddComponent(pFootCollider);
		pFootCollider->SetCollisionRectOffset(3.f, pTexture->GetSize().y - 6.f, true);
		pFootCollider->SetDebugColor("red");
		pFootCollider->SetRenderCollisionBox(true);

		//BulletTimer
		const auto& pTimer = std::make_shared<dae::BulletTimerComponent>(m_pPlayerTwo.get());
		m_pPlayerTwo->AddComponent(pTimer);

		//ShootingDir
		const auto& pShootingDir = std::make_shared<ShootingDirComponent>(m_pPlayerTwo.get());
		m_pPlayerTwo->AddComponent(pShootingDir);

		std::shared_ptr<GameCommands::Stun> stunCommand = std::make_shared<GameCommands::Stun>(m_pPlayerTwo);
		//Movement
		moveCommandUp = std::make_shared<GameCommands::PlayerMovement>(m_pPlayerTwo, m_Up);
		moveCommandDown = std::make_shared<GameCommands::PlayerMovement>(m_pPlayerTwo, m_Down);
		moveCommandLeft = std::make_shared<GameCommands::PlayerMovement>(m_pPlayerTwo, m_Left);
		moveCommandRight = std::make_shared<GameCommands::PlayerMovement>(m_pPlayerTwo, m_Right);

		controllerButton = dae::Controller::ControllerButton::ButtonA;
		dae::InputManager::GetInstance().BindControllerToCommand(controller1Index, controllerButton, stunCommand);
	}
	else
	{
		m_Speed = 65;
		//Texture Verus
		const auto& pTexture = std::make_shared<dae::TextureComponent>(m_pPlayerTwo.get());
		pTexture->SetTexture("Enemies/HotDog.png");
		m_pPlayerTwo->AddComponent(pTexture);

		const auto& pFootCollider = std::make_shared<dae::GameCollisionComponent>(m_pPlayerTwo.get(), m_pPlayerTwo->GetRelativePosition(),
			pTexture->GetSize().x, 8.f, true, true, true);
		m_pPlayerTwo->AddComponent(pFootCollider);
		pFootCollider->SetCollisionRectOffset(3.f, pTexture->GetSize().y - 6.f, true);
		pFootCollider->SetDebugColor("red");
		pFootCollider->SetRenderCollisionBox(true);

		std::shared_ptr<BulletTimerComponent> pTimer = std::make_shared<dae::BulletTimerComponent>(m_pPlayerTwo.get());
		m_pPlayerTwo->AddComponent(pTimer);

		moveCommandUp = std::make_shared<GameCommands::PlayerMovement>(m_pPlayerTwo, m_Up, false);
		moveCommandDown = std::make_shared<GameCommands::PlayerMovement>(m_pPlayerTwo, m_Down, false);
		moveCommandLeft = std::make_shared<GameCommands::PlayerMovement>(m_pPlayerTwo, m_Left, false);
		moveCommandRight = std::make_shared<GameCommands::PlayerMovement>(m_pPlayerTwo, m_Right, false);
	}


	controllerButton = dae::Controller::ControllerButton::DpadUp;
	dae::InputManager::GetInstance().BindControllerToCommand(controller1Index, controllerButton, moveCommandUp);
	controllerButton = dae::Controller::ControllerButton::DpadDown;
	dae::InputManager::GetInstance().BindControllerToCommand(controller1Index, controllerButton, moveCommandDown);
	controllerButton = dae::Controller::ControllerButton::DpadLeft;
	dae::InputManager::GetInstance().BindControllerToCommand(controller1Index, controllerButton, moveCommandLeft);
	controllerButton = dae::Controller::ControllerButton::DpadRight;
	dae::InputManager::GetInstance().BindControllerToCommand(controller1Index, controllerButton, moveCommandRight);

	const auto& pHealth = std::make_shared<dae::HealthComponent>(m_pPlayerTwo.get(), 3);
	m_pPlayerTwo->AddComponent(pHealth);

	const auto& pPoints = std::make_shared<dae::PointComponent>(m_pPlayerTwo.get(), 0);
	m_pPlayerTwo->AddComponent(pPoints);

	scene.Add(m_pPlayerTwo);
}
