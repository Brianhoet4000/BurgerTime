#pragma once
#include "Observer.h"

class PointObserver : public dae::Observer
{
public:
	virtual ~PointObserver() = default;
	void Notify(dae::Event event, dae::GameObject* gameObject) override;

private:
};

