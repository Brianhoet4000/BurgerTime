#pragma once
#include <memory>
#include <glm/vec2.hpp>
#include "BaseComponent.h"
#include "EnemyStates.h"
#include "GameCollisionComponent.h"


namespace dae
{

	class AIMovementComponent : public dae::BaseComponent
	{
	public:
		AIMovementComponent(dae::GameObject* owner, glm::vec2 startPos);
		virtual ~AIMovementComponent() override = default;
		AIMovementComponent(const AIMovementComponent& other) = delete;
		AIMovementComponent(AIMovementComponent&& other) = delete;
		AIMovementComponent& operator=(const AIMovementComponent& other) = delete;
		AIMovementComponent& operator=(AIMovementComponent&& other) = delete;

		void Update(float deltaTime) override;

		void MoveAI(float deltaTime, glm::vec2 dir) const;
		void GetClosestPlayer();

		bool ReturnStunned() const { return m_pEnemyStunned->Stunned(); }
		void Stunned() const { m_pEnemyMovement->OnExit(); m_pEnemyStunned->OnEnter(); }

	private:
		float m_Speed{40.f};
		GameCollisionComponent* m_pCollision;
		std::shared_ptr<dae::BulletTimerComponent> m_pStunTimer{};
		GameObject* m_pClosestPlayer{};

		std::unique_ptr<EnemyMovement> m_pEnemyMovement;
		std::unique_ptr<EnemyStunned> m_pEnemyStunned;
		
	};

}
