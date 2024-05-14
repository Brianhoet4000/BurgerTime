#pragma once
#include <iostream>
#include <vector>
#include <glm/vec2.hpp>
#include "GameCollisionComponent.h"
#include "Singleton.h"

namespace dae
{
	class GameObject;

	class GameCollisionMngr : public dae::Singleton<GameCollisionMngr>
	{
	public:
		GameCollisionMngr() = default;
		virtual ~GameCollisionMngr() = default;
		GameCollisionMngr(const GameCollisionMngr& other) = delete;
		GameCollisionMngr(GameCollisionMngr&& other) = delete;
		GameCollisionMngr& operator=(const GameCollisionMngr& other) = delete;
		GameCollisionMngr& operator=(GameCollisionMngr&& other) = delete;

		void AddCollisionBox(dae::GameObject* owner, GameCollisionComponent* box);
		void RemoveCollisionBox(GameCollisionComponent* box);
		void RemoveBulletBox(GameCollisionComponent* box);
		void RemoveFirstPlayerBox(GameCollisionComponent* box);
		void RemoveSecondPlayerBox(GameCollisionComponent* box);
		void RemoveEnemyBox(GameCollisionComponent* box);
		void ClearAll();

		void PrintIngredients() const { std::cout << m_pIngredientBoxes.size() << '\n'; }

		std::vector<GameCollisionComponent*> GetAllWall();
		std::vector<GameCollisionComponent*> GetAllStairs();
		std::vector<GameCollisionComponent*> GetAllFloors();
		std::vector<GameCollisionComponent*> GetAllEnemies();
		GameCollisionComponent* GetFirstPlayer() const { return m_pFirstPlayer; }
		GameCollisionComponent* GetSecondPlayer() const { return m_pSecondPlayer; }
		std::vector<GameCollisionComponent*> GetAllPlayers() const;

		GameCollisionComponent* CheckForCollisionComponent(const GameCollisionComponent* box) const;
		GameCollisionComponent* CheckOverlapWithEnemiesComponent(const GameCollisionComponent* box) const;

		GameCollisionComponent* CheckOverlapWithSecondPlayerVersus(const GameCollisionComponent* box) const;
		GameCollisionComponent* CheckOverlapWithFirstPlayer(const GameCollisionComponent* box) const;
		GameCollisionComponent* CheckOverlapWithPlayers(const GameCollisionComponent* box) const;
		bool CheckOverlapWithPlayersBool(const GameCollisionComponent* box) const;
		bool CheckOverlapWithPlayersBoolIngredients(const GameCollisionComponent* box) const;
		bool CheckForInStairsX() const;
		bool CheckOverlapWithStairs() const;
		bool MovePlayerUpStairs() const;
		bool MovePlayerDownStairs() const;
		bool CheckOverlapWithFloors() const;
		bool MovePlayerLeftFloors() const;
		bool MovePlayerRightFloors() const;

		bool CheckOverlapWithIngredient() const;

		
		bool CheckForCollision(const GameCollisionComponent* box) const;
		bool CheckForOverlapWall(const dae::GameCollisionComponent* box) const;
		bool CheckOverlapWithEnemies(const GameCollisionComponent* box) const;

		void PlayerLogicBox(dae::GameCollisionComponent* box, glm::vec2 dir);

		bool Raycast(glm::vec2 startpos, glm::vec2 direction,const dae::GameCollisionComponent* box) const;

		bool AIRaycast(glm::vec2 startpos, glm::vec2 direction, const dae::GameCollisionComponent* box) const;
		bool AIRaycastUp(glm::vec2 startpos, glm::vec2 direction, const dae::GameCollisionComponent* box) const;
		bool AIRaycastRight(glm::vec2 startpos, glm::vec2 direction, const dae::GameCollisionComponent* box) const;
		bool AIRaycastLeft(glm::vec2 startpos, glm::vec2 direction, const dae::GameCollisionComponent* box) const;
		bool AIRaycastDown(glm::vec2 startpos, glm::vec2 direction, const dae::GameCollisionComponent* box) const;

	private:
		std::vector<GameCollisionComponent*> m_pCollisonBoxes;
		std::vector<GameCollisionComponent*> m_pWallBoxes;
		std::vector<GameCollisionComponent*> m_pStairsBoxes;
		std::vector<GameCollisionComponent*> m_pFloorBoxes;
		std::vector<GameCollisionComponent*> m_pIngredientBoxes;
		std::vector<GameCollisionComponent*> m_pEnemies;
		GameCollisionComponent* m_pFirstPlayer{nullptr};
		GameCollisionComponent* m_pSecondPlayer{nullptr};

		const float m_Dim = 24.f;
		const int m_Volume{ 5 };
	};

}