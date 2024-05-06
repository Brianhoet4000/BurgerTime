#pragma once
#include <SDL.h>
#include "Singleton.h"
#include <vector>

namespace dae
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{};	
	public:
		void Init(SDL_Window* window);
		void Render();
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;

		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }

	private:
		void DrawPlotTrashCache();
		void DrawPlotGameObject3D();
		std::vector<float> m_TrashCacheData{};
		int m_TrashCacheDataSize{};
		int m_IndexSteps{};
		std::vector<float> m_TrashCacheDataObject3D{};
		std::vector<float> m_TrashCacheDataObject3DAlt{};
		int m_TrashCacheDataSizeObject3D{};
		int m_SizeObject3DAlt{};
		int m_IndexObject3D{};
	};
}

