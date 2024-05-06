#pragma once
#include <string>
#include <memory>
#include "BaseComponent.h"
#include "Texture2D.h"

namespace dae
{

	class Texture2D;

	class TextureComponent : public BaseComponent
	{
	public:
		virtual void Update(float) override;
		virtual void Render() const override;

		void SetTexture(const std::string& filename);

		TextureComponent(GameObject* owner);
		virtual ~TextureComponent() = default;
		TextureComponent(const TextureComponent& other) = delete;
		TextureComponent(TextureComponent&& other) = delete;
		TextureComponent& operator=(const TextureComponent& other) = delete;
		TextureComponent& operator=(TextureComponent&& other) = delete;
		glm::vec2 GetSize();

		void SetMustRender(bool render);
		bool GetMustRender() const;

	private:
		std::shared_ptr<dae::Texture2D> m_Texture{};
		bool m_MustTexture{ true };
	};
}