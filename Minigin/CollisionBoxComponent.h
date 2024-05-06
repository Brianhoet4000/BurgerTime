#pragma once
#include <SDL_rect.h>
#include "BaseComponent.h"


namespace dae
{

	class CollisionBoxComponent : public BaseComponent
	{
	public:

		CollisionBoxComponent(GameObject* owner);
		virtual ~CollisionBoxComponent() = default;
		CollisionBoxComponent(const CollisionBoxComponent& other) = delete;
		CollisionBoxComponent(CollisionBoxComponent&& other) = delete;
		CollisionBoxComponent& operator=(const CollisionBoxComponent& other) = delete;
		CollisionBoxComponent& operator=(CollisionBoxComponent&& other) = delete;

		virtual void Update(float) override;
		virtual void Render() const override;
		SDL_FRect GetCollisionRect() const;
		void SetCollisionRect(float Offset);

		GameObject* GetOwner() const;

		bool GetRenderCollisionBox() const { return m_RenderCollisionBox; }
		void SetRenderCollisionBox(bool RenderCollisionBox) { m_RenderCollisionBox = RenderCollisionBox; }

	private:
		SDL_FRect m_CollisionBox;
		bool m_RenderCollisionBox = false;
		float m_Offset = 0;
	};

}
