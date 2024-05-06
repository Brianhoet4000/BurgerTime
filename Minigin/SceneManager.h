#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void Update(float deltaTime) const;
		void FixedUpdate(float fixedTimeStep) const;
		void Render() const;

		void NextScene();
		void SetActiveScene(const std::string& sceneName);
		Scene* GetActiveScene() const { return m_pScenes[m_ActiveScene].get(); }
		std::string GetActiveSceneName() const;
		void IncrementActivescene() { ++m_ActiveScene; }

		std::vector<std::shared_ptr<Scene>> GetAllScene() { return m_pScenes; }

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_pScenes;

		int m_ActiveScene{ 0 };
	};
}
