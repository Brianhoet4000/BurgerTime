#pragma once
#include <map>
#include <SDL_rect.h>
#include <string>
#include <glm/vec2.hpp>

#include "BaseComponent.h"

namespace dae
{
	struct Color
	{
		Uint8 r;
		Uint8 g;
		Uint8 b;
		Uint8 a = 255;
	};

	class GameCollisionComponent : public BaseComponent
	{
	public:

		GameCollisionComponent(GameObject* owner, glm::vec2 pos, const float width, const float height, bool needsUpdate = false, bool visualizeRect = false, bool IsVersus = false);
		virtual ~GameCollisionComponent() = default;
		GameCollisionComponent(const GameCollisionComponent& other) = delete;
		GameCollisionComponent(GameCollisionComponent&& other) = delete;
		GameCollisionComponent& operator=(const GameCollisionComponent& other) = delete;
		GameCollisionComponent& operator=(GameCollisionComponent&& other) = delete;

		virtual void Update(float) override;
		virtual void Render() const override;
		SDL_FRect GetCollisionRect() const;
		void SetCollisionRectOffset(float Offset);
		void SetCollisionRectOffset(float Offsetx, float Offsety, bool centerizedOffsetX = false, bool centerizedOffsetY = false);

		GameObject* GetOwner() const;

		bool GetRenderCollisionBox() const { return m_RenderCollisionBox; }
		void SetRenderCollisionBox(bool RenderCollisionBox) { m_RenderCollisionBox = RenderCollisionBox; }

		bool GetIsVersus() const { return m_IsVersus; }


		void SetDebugColor(const Color& color)
		{
			m_Color = color;
		}
		void SetDebugColor(const std::string& colorName)
		{
			auto it = m_ColorMap.find(colorName);
			if (it != m_ColorMap.end())
			{
				m_Color = it->second;
			}
			else
			{
				// Default to white if the color name is not found
				m_Color = m_White;
			}
		}

	private:
		// Define predefined colors
		const Color m_Red{ 255, 0 ,0 };
		const Color m_Green{ 0, 255, 0 };
		const Color m_Blue{ 0, 0, 255 };
		const Color m_White{ 255, 255, 255 };
		std::map<std::string, Color> m_ColorMap{
			{"red", m_Red},
			{"green", m_Green},
			{"blue", m_Blue},
			{"white", m_White}
		};

		//private
		SDL_FRect m_CollisionBox;
		bool m_RenderCollisionBox = false;
		glm::vec2 m_Offset = glm::vec2{ 0, 0 };
		bool m_IsVersus;
		bool m_NeedsUpdate;
		glm::vec2 m_Pos;
		Color m_Color = m_Green;
	};

}
