#pragma once
#include "State.h"

class NobbinState : public dae::State
{
	void OnEnter() override;
	void Update() override;
	void OnExit() override;
};

