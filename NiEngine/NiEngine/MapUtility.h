#pragma once

#include <unordered_map>
#include <vector>

namespace ni {

struct MapUtility
{
	template <typename Key, typename Value>
	static std::vector<Value> MapToVector(const std::unordered_map<Key, Value>& map)
	{
		std::vector<Value> result;
		for (auto& [k, v] : map)
		{
			result.emplace_back(v);
		}
		return result;
	}
};

}