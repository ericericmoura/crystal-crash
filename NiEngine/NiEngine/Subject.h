#pragma once

#include <utility>
#include <functional>
#include <algorithm>
#include <vector>

namespace ni {

template <typename ...Args>
class Subject
{
private:
	std::vector<std::pair<int, std::function<void(Args...)>>> observers_;
	int next_id_ = 0;

public:
	int Subscribe(std::function<void(Args...)> on_event);
	
	void Remove(int id);
	void Notify(Args&&... args);
};

template<typename ...Args>
inline int Subject<Args...>::Subscribe(std::function<void(Args...)> on_event)
{
	observers_.emplace_back(++next_id_, std::move(on_event));
	return next_id_;
}

template<typename ...Args>
inline void Subject<Args...>::Remove(int id)
{
	for (size_t i = 0; i < observers_.size(); ++i)
	{
		if (observers_.at(i).first == id)
		{
			observers_.erase(observers_.begin() + i);
			break;
		}
	}
}

template<typename ...Args>
inline void Subject<Args...>::Notify(Args && ...args)
{
	for (const auto& [id, on_event] : observers_)
	{
		on_event(std::forward<Args>(args)...);
	}
}

}