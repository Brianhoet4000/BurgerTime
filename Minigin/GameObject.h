#pragma once
#include <memory>
#include "Transform.h"
#include <vector>
#include <iostream>
#include "BaseComponent.h"
#include "Subject.h"

namespace dae
{
	class Scene;
	class BaseComponent;

	class GameObject final
	{
	public:

		GameObject(const std::string& tag = "TagLess");
		~GameObject() = default;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Update(float deltaTime) const;
		void FixedUpdate(float deltaTime);
		void Render() const;

		void AddComponent(std::shared_ptr<BaseComponent> component);
		void RemoveComponent(std::shared_ptr<BaseComponent> component);
		

		template <typename T>
		T* GetComponent()
		{
			for (const auto& component : m_pComponents)
			{
				if (typeid(*component) == typeid(T))
				{
					return static_cast<T*>(component.get());
				}
			}
			return nullptr;
		}
		
		void UpdateWorldPos();
		const glm::vec2& GetWorldPosition();

		GameObject* GetParent() const;
		std::vector<std::shared_ptr<GameObject>> GetChildren() const;
		GameObject* GetChildAt(const unsigned int idx) const { return m_pChildren[idx].get(); }
		void RemoveChild(const GameObject* child);
		void RemoveAllChilderen();
		void SetParent(GameObject* parent);
		void AddChild(const std::shared_ptr<GameObject>& child);
		void RemoveAllComponents();

		glm::vec2 GetRelativePosition() const;	
		void SetRelativePosition(glm::vec2 Pos);
		void SetRelativePosition(float Posx, float Posy);

		void FlagIsTrue();
		std::string GetTag() const;
		void SetTag(const std::string& tag);

		void MarkTrueForDeleting();
		bool ReturnDeleting() const;
		void SetUpdate(bool update) { m_Update = update; }

		void SetScene(Scene* scene);
		Scene* GetScene() const;

		std::vector<std::shared_ptr<BaseComponent>> GetAllComponents() { return m_pComponents; }
		std::vector<std::shared_ptr<GameObject>> GetAllChildren() { return m_pChildren; }

	protected:
		std::unique_ptr<Subject> m_pSubject;

	private:
		glm::vec2 m_WorldTransform{};
		glm::vec2 m_RelativeTransform{};

		std::vector<std::shared_ptr<BaseComponent>> m_pComponents{};

		GameObject* m_pParent{ nullptr };
		std::vector<std::shared_ptr<GameObject>> m_pChildren{};

		bool m_Update{ true };
		bool m_DeletParentFromScene{ false };
		bool m_DirtyFlag{ false };
		std::string m_TagString;

		Scene* m_pScene{};
	};
}
