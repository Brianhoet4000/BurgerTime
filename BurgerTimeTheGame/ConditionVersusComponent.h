#pragma once
#include "BaseComponent.h"

namespace dae
{

	class ConditionVersusComponent : public dae::BaseComponent
	{
	public:

		ConditionVersusComponent(dae::GameObject* owner);
		virtual ~ConditionVersusComponent() = default;
		ConditionVersusComponent(const ConditionVersusComponent& other) = delete;
		ConditionVersusComponent(ConditionVersusComponent&& other) = delete;
		ConditionVersusComponent& operator=(const ConditionVersusComponent& other) = delete;
		ConditionVersusComponent& operator=(ConditionVersusComponent&& other) = delete;

		void Update(float) override;

	private:
		bool m_Finished;
		bool m_DoOnce = false;
	};

}