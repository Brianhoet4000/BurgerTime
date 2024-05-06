#include "Subject.h"
#include "Observer.h"

dae::Subject::Subject(dae::GameObject * owner)
{
    m_pOwner = owner;
}

void dae::Subject::AddObserver(std::shared_ptr<Observer> observer)
{
    m_pObservers.push_back(observer);
}

void dae::Subject::RemoveObserver(std::shared_ptr<Observer> observer)
{
    m_pObservers.erase(std::remove(m_pObservers.begin(), m_pObservers.end(), observer), m_pObservers.end());
}

void dae::Subject::NotifyObservers(dae::Event event)
{
    for (const auto& observer : m_pObservers)
    {
        observer->Notify(event, m_pOwner);
    }
}
