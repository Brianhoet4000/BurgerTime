#pragma once
#include "BaseComponent.h"

namespace dae
{

	class ShootingDirComponent : public BaseComponent
	{
	public:

		ShootingDirComponent(GameObject* owner)
			:BaseComponent(owner)
		{
		}
		~ShootingDirComponent() = default;

		enum FaceState
		{
			Up,
			Right,
			Down,
			Left
		};

		FaceState returnFaceState() const { return m_FaceState; }
		void SetFaceState(FaceState state) { m_FaceState = state; }

	private:
		FaceState m_FaceState{ Right };
	};

}