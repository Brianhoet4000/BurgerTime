#pragma once
#include "State.h"

class HobbinState : public dae::State
{
	void OnEnter() override;
	void Update(float deltaTime) override;
	void OnExit() override;
};

