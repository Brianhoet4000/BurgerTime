#pragma once
#include <glm/vec2.hpp>

#include "BulletTimerComponent.h"
#include "GameObject.h"
#include "State.h"

class EnemyMovement : public dae::State
{
public:
	EnemyMovement(dae::GameObject* owner, glm::vec2 startPos);

	void OnEnter() override;
	void Update(float deltaTime) override;
	void OnExit() override;

private:
	bool m_update = false;

	glm::vec2 m_Dir = { 0,0 };
	glm::vec2 m_DirLeft{ -1,0 };
	glm::vec2 m_DirRight{ 1, 0 };
	glm::vec2 m_DirUp{ 0, -1 };
	glm::vec2 m_DirDown{ 0, 1 };

	dae::GameObject* m_pOwner;

	glm::vec2 m_LeftOfStartPos;
	glm::vec2 m_RightOfStartPos;
};

class EnemyStunned :public dae::State
{
public:
	EnemyStunned(dae::GameObject* owner);

	void OnEnter() override;
	void Update(float deltaTime) override;
	void OnExit() override;

	inline bool Stunned() const { return m_update; }
	inline bool DoneStun() const { return m_pStunTimer->ReturnCompleted(); }

private:
	bool m_update = false;

	dae::GameObject* m_pOwner;

	std::shared_ptr<dae::BulletTimerComponent> m_pStunTimer{};
};