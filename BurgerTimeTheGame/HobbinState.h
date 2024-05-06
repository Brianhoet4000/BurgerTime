#pragma once
#include "State.h"

class HobbinState : public dae::State
{
	void OnEnter() override;
	void Update() override;
	void OnExit() override;
};

