#pragma once
#include "State.h"

class NobbinState : public dae::State
{
	void OnEnter() override;
	void Update(float deltaTime) override;
	void OnExit() override;
};

