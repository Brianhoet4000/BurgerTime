#pragma once
#include "BulletTimerComponent.h"
#include "Commands.h"
#include "GameCollisionComponent.h"
#include "LevelPrefab.h"
#include "ScreenManager.h"

namespace GameCommands
{

	class PlayerMovement : public dae::Command
	{
	private:
		glm::vec2 m_Dir{};
		dae::GameCollisionComponent* m_pCollision;
		bool m_Player;
	public:
		PlayerMovement(std::shared_ptr<dae::GameObject> owner, const glm::vec2& dir, bool player = true);
		virtual void Execute(float deltaTime) override;
	};

	class Stun : public dae::Command
	{
	private:
		glm::vec2 m_Dir{};
		dae::BulletTimerComponent* m_pBulletTimer{};

	public:
		Stun(std::shared_ptr<dae::GameObject> owner);
		virtual void Execute(float deltaTime) override;
	};

	class SwitchGameMode : public dae::Command
	{
	private:
		std::shared_ptr<dae::GameObject> m_pScreen{};
		dae::GameObject* m_pTextMode;
		dae::ScreenManager* m_pScreenManager{};
		dae::ScreenManager::GameMode& m_CurrentScreen;
	public:
		SwitchGameMode(std::shared_ptr<dae::GameObject> owner, dae::GameObject* text, dae::ScreenManager::GameMode& currentScreen, dae::ScreenManager* screen);
		virtual void Execute(float) override;

	};

	class AcceptGameMode : public dae::Command
	{
	private:
	public:
		AcceptGameMode() = default;
		virtual void Execute(float) override;
	};

	class SkipLevel : public dae::Command
	{
	private:
		bool m_CreatedEndScreen = false;
	public:
		SkipLevel() = default;
		virtual void Execute(float) override;
	};

	class ResetLevel : public dae::Command
	{
	private:

	public:
		ResetLevel() = default;
		virtual void Execute(float) override;
	};

	class MuteMusic : public dae::Command
	{
	private:

	public:
		MuteMusic() = default;
		virtual void Execute(float) override;
	};

}
