#pragma once
#include "GameObject.h"
#include "LevelPrefab.h"
#include "Scene.h"

namespace dae
{
	class PlayerOne final
	{
	public:
		PlayerOne(dae::Scene& scene);

		~PlayerOne() = default;
		PlayerOne(const PlayerOne& other) = delete;
		PlayerOne(PlayerOne&& other) = delete;
		PlayerOne& operator=(const PlayerOne& other) = delete;
		PlayerOne& operator=(PlayerOne&& other) = delete;

		std::shared_ptr<dae::GameObject> ReturnPlayer() { return m_pPlayerOne; }
		

	private:
		const float m_Speed{ 40.f };
		glm::vec2 m_Up = { 0.f,-m_Speed };
		glm::vec2 m_Down = { 0.f,m_Speed };
		glm::vec2 m_Right = { m_Speed,0.f };
		glm::vec2 m_Left = { -m_Speed,0.f };

		std::shared_ptr<dae::GameObject> m_pPlayerOne;
		int m_Controller1Index{ 0 };
	};
}