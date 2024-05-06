#pragma once
#include "BaseComponent.h"
#include "TextureComponent.h"

namespace dae
{

	class HobbinComponent : public dae::BaseComponent
	{
	public:
		enum CharacterState
		{
			Nobbin,
			Hobbin
		};

		HobbinComponent(dae::GameObject* owner);

		virtual ~HobbinComponent() override = default;
		HobbinComponent(const HobbinComponent& other) = delete;
		HobbinComponent(HobbinComponent&& other) = delete;
		HobbinComponent& operator=(const HobbinComponent& other) = delete;
		HobbinComponent& operator=(HobbinComponent&& other) = delete;

		void Update(float deltaTime) override;

		CharacterState ReturnCharacterState() const { return m_CharacterState; }

	private:
		bool m_StartTimer = true;
		bool m_TimerDone = false;
		float m_Startvalue = 10.f;
		float m_Countdownvalue = 0.f;

		CharacterState m_CharacterState{ Nobbin };
		dae::TextureComponent* m_pTexture;
	};

}
