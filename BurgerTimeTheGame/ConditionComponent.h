#pragma once
#include <memory>

#include "BaseComponent.h"

namespace dae
{

	class ConditionComponent : public dae::BaseComponent
	{
	public:

		ConditionComponent(dae::GameObject* owner);
		virtual ~ConditionComponent() = default;
		ConditionComponent(const ConditionComponent& other) = delete;
		ConditionComponent(ConditionComponent&& other) = delete;
		ConditionComponent& operator=(const ConditionComponent& other) = delete;
		ConditionComponent& operator=(ConditionComponent&& other) = delete;

		void Update(float) override;

	private:
		bool m_Finished;
		//std::shared_ptr<dae::GameObject> m_pSpawner;
		bool m_DoOnce = false;
	};

}
