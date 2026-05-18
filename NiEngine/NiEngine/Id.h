#pragma once

#include <cstdint>
#include <type_traits>

namespace ni {

template <typename Tag>
struct Id
{
	uint32_t id_ = -1;

	bool operator==(const Id& other) const
	{
		return id_ == other.id_;
	}

	Id& operator++()
	{
		id_++;
		return *this;
	}
};

}

template <typename Tag>
struct std::hash<ni::Id<Tag>>
{
	size_t operator()(const ni::Id<Tag>& id) const
	{
		return std::hash<uint32_t>{}(id.id_);
	}
};