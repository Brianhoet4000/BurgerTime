#pragma once
#include "Observer.h"

class HealthObserver : public dae::Observer
{
public:
	virtual ~HealthObserver() = default;
	void Notify(dae::Event event, dae::GameObject* gameObject) override;

private:
};

