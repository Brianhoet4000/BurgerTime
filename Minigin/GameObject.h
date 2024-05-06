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

		void Update(float deltaTime);
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
		void RemoveChild(const GameObject* child);
		void RemoveAllChilderen();
		void SetParent(GameObject* parent);
		void AddChild(const std::shared_ptr<GameObject>& child);
		

		glm::vec2 GetRelativePosition() const;	
		void SetRelativePosition(glm::vec2 Pos);
		void SetRelativePosition(float Posx, float Posy);

		void FlagIsTrue();
		std::string GetTag() const;
		void SetTag(const std::string& tag);

		void MarkTrueForDeleting();
		bool ReturnDeleting();

		void SetScene(Scene* scene);
		Scene* GetScene() const;

	protected:
		std::unique_ptr<Subject> m_pSubject;

	private:
		glm::vec2 m_WorldTransform{};
		glm::vec2 m_RelativeTransform{};

		std::vector<std::shared_ptr<BaseComponent>> m_pComponents{};

		GameObject* m_pParent{ nullptr };
		std::vector<std::shared_ptr<GameObject>> m_pChildren{};

		bool m_deletParentFromScene{ false };
		bool m_dirtyFlag{ false };
		std::string m_TagString;

		Scene* m_pScene{};
	};
}
