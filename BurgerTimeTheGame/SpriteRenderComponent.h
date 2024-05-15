#pragma once
#include <memory>
#include <vector>
#include <glm/vec4.hpp>

#include "BaseComponent.h"
#include "Texture2D.h"

namespace dae
{
	struct SpriteSheet
	{
		std::shared_ptr<Texture2D> pTexture{};
		int rows{}, columns{};
		float animTimer{};
	};

	class SpriteRenderComponent : public BaseComponent
	{
	public:
		SpriteRenderComponent(dae::GameObject* owner, SpriteSheet sprite);

		virtual ~SpriteRenderComponent() override = default;
		SpriteRenderComponent(const SpriteRenderComponent& other) = delete;
		SpriteRenderComponent(SpriteRenderComponent&& other) = delete;
		SpriteRenderComponent& operator=(const SpriteRenderComponent& other) = delete;
		SpriteRenderComponent& operator=(SpriteRenderComponent&& other) = delete;
		void Update(float) override;
		void Render() const override;
		bool CanRender() const { return !m_Stop; }

		void SelectSprite(int idx);

		void PlayAnimation(int startIdx, int endIdx, int loops = -1);
		void PlayAnimation(const std::vector<int>& idcs, int loops = -1);
		void UpdateAnimation(int startIdx, int endIdx, int loops = -1);
		bool IsAnimationPlaying() const { return m_IsAnimationPlaying; }
		void Pause(bool value);
		void Stop(bool value);
		//void FlipTexture(RenderFlip flip);

		glm::ivec2 GetSpriteSize() const;

	private:
		SpriteSheet m_SpriteSheet;

		int m_CurrIdx{};
		int m_StartIdx{};
		int m_EndIdx{};
		bool m_PlaySelectedIdcs{};
		std::vector<int> m_SelectedIdcs{};

		int m_Loops{};
		int m_AccuLoops{};

		float m_AccuTime{};

		bool m_Pause{};
		bool m_IsAnimationPlaying{};
		bool m_Stop{};

		glm::vec2 m_RenderPos{};
		glm::ivec4 m_Rect{};
		//RenderFlip m_Flip{ RenderFlip::none };
	};

}
