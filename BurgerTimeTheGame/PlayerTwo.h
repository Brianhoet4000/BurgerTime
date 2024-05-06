#pragma once
#include "GameObject.h"
#include "LevelPrefab.h"
#include "Scene.h"

namespace dae
{
	class PlayerTwo final
	{
	public:
		PlayerTwo(dae::Scene& scene, dae::LevelPrefab& level, bool Coop);

		~PlayerTwo() = default;
		PlayerTwo(const PlayerTwo& other) = delete;
		PlayerTwo(PlayerTwo&& other) = delete;
		PlayerTwo& operator=(const PlayerTwo& other) = delete;
		PlayerTwo& operator=(PlayerTwo&& other) = delete;

		void SetCoopBool(bool Coop)
		{
			if(Coop)
			{
				m_pPlayerTwo->GetComponent<TextComponent>();
			}
		}

		std::shared_ptr<GameObject> ReturnPlayer()
		{
			return m_pPlayerTwo;
		}

	private:
		float m_Speed{ 50.f };
		glm::vec2 m_Up = { 0.f,-m_Speed };
		glm::vec2 m_Down = { 0.f,m_Speed };
		glm::vec2 m_Right = { m_Speed,0.f };
		glm::vec2 m_Left = { -m_Speed,0.f };

		std::shared_ptr<dae::GameObject> m_pPlayerTwo;
	};

}