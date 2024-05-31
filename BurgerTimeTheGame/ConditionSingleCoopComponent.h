#pragma once
#include <memory>

#include "BaseComponent.h"

namespace dae
{

	class ConditionSingleCoopComponent : public dae::BaseComponent
	{
	public:

		ConditionSingleCoopComponent(dae::GameObject* owner);
		virtual ~ConditionSingleCoopComponent() = default;
		ConditionSingleCoopComponent(const ConditionSingleCoopComponent& other) = delete;
		ConditionSingleCoopComponent(ConditionSingleCoopComponent&& other) = delete;
		ConditionSingleCoopComponent& operator=(const ConditionSingleCoopComponent& other) = delete;
		ConditionSingleCoopComponent& operator=(ConditionSingleCoopComponent&& other) = delete;

		void Update(float) override;

	private:
		bool m_Finished;
		//std::shared_ptr<dae::GameObject> m_pSpawner;
		bool m_DoOnce = false;
	};

}
