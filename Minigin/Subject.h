#pragma once
#include "Event.h"

#include <vector>
#include <iostream>


namespace dae
{
	class GameObject;


	class Observer;

	class Subject
	{
	public:
		Subject(dae::GameObject* owner);
		~Subject() = default;

		void AddObserver(std::shared_ptr<Observer> observer);
		void RemoveObserver(std::shared_ptr<Observer> observer);
		void NotifyObservers(Event event);

	private:
		dae::GameObject* m_pOwner;
		std::vector<std::shared_ptr<Observer>> m_pObservers;

	};

}