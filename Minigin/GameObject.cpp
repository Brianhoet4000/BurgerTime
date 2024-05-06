#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include <algorithm>

#include "CollisionBoxComponent.h"
#include "CollisionBoxManager.h"

namespace dae
{
	GameObject::GameObject(const std::string& tag)
		:m_TagString{ tag }
	{
	}

	void GameObject::Update(float deltaTime)
	{
		

		for (const auto& pComponent : m_pComponents)
		{
			if (pComponent == nullptr) return;

			pComponent->Update(deltaTime);
		}

		for (const auto& child : m_pChildren)
		{
			if (child == nullptr) return;

			child->Update(deltaTime);
		}
	}

	void GameObject::FixedUpdate(float deltaTime)
	{
		for (const auto& pComponent : m_pComponents)
		{
			pComponent->FixedUpdate(deltaTime);
		}

		for (const auto& child : m_pChildren)
		{
			child->FixedUpdate(deltaTime);
		}
	}

	void GameObject::Render() const
	{
		for (const auto& pComponent : m_pComponents)
		{
			pComponent->Render();
		}

		for(const auto& child : m_pChildren)
		{
			if (child->ReturnDeleting()) continue;

			child->Render();
		}
	}

	void GameObject::UpdateWorldPos()
	{
		if (m_dirtyFlag)
		{
			if (m_pParent == nullptr)
				m_WorldTransform = m_RelativeTransform;
			else
				m_WorldTransform = m_pParent->GetWorldPosition() + m_RelativeTransform;

			m_dirtyFlag = false;
		}
	}

	void GameObject::AddComponent(std::shared_ptr<BaseComponent> component)
	{
		m_pComponents.push_back(component);
	}

	void dae::GameObject::RemoveComponent(std::shared_ptr<BaseComponent> component)
	{
		m_pComponents.erase(std::remove(m_pComponents.begin(), m_pComponents.end(), component), m_pComponents.end());
	}

	GameObject* GameObject::GetParent() const
	{
		return m_pParent;
	}

	std::vector<std::shared_ptr<GameObject>> GameObject::GetChildren() const
	{
		return m_pChildren;
	}

	void GameObject::SetParent(GameObject* parent)
	{
		if(m_pParent)
		{
			m_pParent->RemoveChild(this);
		}
		m_pParent = parent;

		UpdateWorldPos();

	}

	void GameObject::AddChild(const std::shared_ptr<GameObject>& child)
	{
		m_pChildren.push_back(child);
		child->SetParent(this);
	}

	void GameObject::RemoveChild(const GameObject* child)
	{
		m_pChildren.erase(std::remove_if(m_pChildren.begin(), m_pChildren.end(),
			[&](const std::shared_ptr<GameObject>& ptr) {
				return ptr.get() == child;
			}), m_pChildren.end());
	}

	void GameObject::RemoveAllChilderen()
	{
		m_pChildren.clear();
	}

	const glm::vec2& GameObject::GetWorldPosition()
	{
		if (m_dirtyFlag)
			UpdateWorldPos();
		return m_WorldTransform;
	}

	glm::vec2 GameObject::GetRelativePosition() const
	{
		return m_RelativeTransform;
	}

	void GameObject::SetRelativePosition(glm::vec2 Pos)
	{
		m_RelativeTransform = Pos;

		if(m_pParent == nullptr)
		{
			m_WorldTransform = m_RelativeTransform;
		}

		FlagIsTrue();
	}

	void GameObject::SetRelativePosition(float Posx, float Posy)
	{
		m_RelativeTransform.x = Posx;
		m_RelativeTransform.y = Posy;

		if (m_pParent == nullptr)
		{
			m_WorldTransform = m_RelativeTransform;
		}

		FlagIsTrue();
	}

	void GameObject::FlagIsTrue()
	{
		m_dirtyFlag = true;
		for (const auto& child : m_pChildren)
		{
			child->FlagIsTrue();
		}
	}

	std::string GameObject::GetTag() const
	{
		return m_TagString;
	}

	void GameObject::SetTag(const std::string& tag)
	{
		m_TagString = tag;
	}

	void GameObject::MarkTrueForDeleting()
	{
		m_deletParentFromScene = true;
	}

	bool GameObject::ReturnDeleting()
	{
		return m_deletParentFromScene;
	}

	void GameObject::SetScene(Scene* scene)
	{
		m_pScene = scene;
		for (const auto& c : m_pChildren)
		{
			c->SetScene(scene);
		}
	}

	Scene* GameObject::GetScene() const
	{
		return m_pScene;
	}

}
