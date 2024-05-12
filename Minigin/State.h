#pragma once
namespace dae
{
	class State
	{
	public:
		virtual ~State() = default;

		virtual void OnEnter() = 0;
		virtual void Update(float deltaTime) = 0;
		virtual void OnExit() = 0;
	};
}