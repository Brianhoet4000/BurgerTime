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
	void MoveAI(float deltaTime, glm::vec2 dir) const
	{
		m_pOwner->SetRelativePosition(m_pOwner->GetRelativePosition() + dir * m_Speed * deltaTime);
	}
	void GetClosestPlayer();
	void OnExit() override;
	void ClampPositionToStairsOrFloor();

private:
	bool m_update = false;
	bool m_OnStairs = false;
	bool m_OnFloor = false;
	float m_Speed{ 35.f };
	glm::vec2 m_Dir = { 0,0 };
	glm::vec2 m_DirLeft{ -1,0 };
	glm::vec2 m_DirRight{ 1, 0 };
	glm::vec2 m_DirUp{ 0, -1 };
	glm::vec2 m_DirDown{ 0, 1 };
	dae::GameObject* m_pOwner;
	dae::GameObject* m_pClosestPlayer{};
};

class EnemyStunned :public dae::State
{
public:
	EnemyStunned(dae::GameObject* owner, dae::BulletTimerComponent* timer);

	void OnEnter() override;
	void Update(float deltaTime) override;
	void OnExit() override;

	inline bool Stunned() const { return m_update; }
	inline bool DoneStun() const { return m_pStunTimer->ReturnCompleted(); }

private:
	bool m_update = false;

	dae::GameObject* m_pOwner;

	dae::BulletTimerComponent* m_pStunTimer{};
};