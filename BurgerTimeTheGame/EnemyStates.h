#pragma once
#include "State.h"

class EnemyMovement : public dae::State
{
public:
	void OnEnter() override;
	void Update(float deltaTime) override;
	void OnExit() override;

private:
};

class EnemyStunned :public dae::State
{
public:
	void OnEnter() override;
	void Update(float deltaTime) override;
	void OnExit() override;

private:
};