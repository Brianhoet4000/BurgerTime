#pragma once
#include <queue>
#include <functional>
#include <type_traits>
template<typename EventType, typename TimeType>
class EventQueue
{
public:
    static_assert(std::is_default_constructible_v<EventType>, "EventType must be default constructible");

    static EventQueue& GetInstance()
	{
        static EventQueue instance;
        return instance;
    }

    void schedule(const EventType& event, TimeType time)
	{
        queue.push(std::make_pair(time, event));
    }

    void process()
	{
        while (!queue.empty())
        {
            auto [time, event] = queue.top();
            if (time > currentTime)
            {
                currentTime = time;
            }
            queue.pop();
            event.execute();
        }
    }

    bool has_events() const
	{
        return !queue.empty();
    }

    TimeType getCurrent_time() const
	{
        return currentTime;
    }

    TimeType next_time() const
	{
        if (queue.empty())
        {
            return currentTime;
        }
        return queue.top().first;
    }

    void process_next()
	{
        if (!queue.empty())
        {
            auto [time, event] = queue.top();
            currentTime = time;
            queue.pop();
            event.execute();
        }
    }

private:
    EventQueue() = default;
    EventQueue(const EventQueue&) = delete;
    EventQueue(EventQueue&&) = delete;
    EventQueue& operator=(const EventQueue&) = delete;
    EventQueue& operator=(EventQueue&&) = delete;

    TimeType currentTime{};

    struct EventComparator {
        bool operator()(const std::pair<TimeType, EventType>& lhs, const std::pair<TimeType, EventType>& rhs) const {
            return lhs.first > rhs.first;
        }
    };

    std::priority_queue<std::pair<TimeType, EventType>, std::vector<std::pair<TimeType, EventType>>, EventComparator> queue;
};



