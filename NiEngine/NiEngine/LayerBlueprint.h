#pragma once

#include <string>
#include <vector>
#include <type_traits>

#include <SFML/System/Vector2.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace ni {

struct LayerBlueprint
{
	std::string type_ = "";
	std::string name_ = "";
	sf::Vector2f position_ = {};

	std::vector<int> data_ = {};
	bool visible_ = false;

	float opacity_ = 0;

	inline bool operator==(const LayerBlueprint& b) const
	{
		return type_ == b.type_
			&& name_ == b.name_
			&& position_.x == b.position_.x
			&& position_.y == b.position_.y
			&& visible_ == b.visible_
			&& opacity_ == b.opacity_
			&& data_ == b.data_;
	}
};

struct LayerBlueprintHash
{
	std::size_t operator()(const LayerBlueprint& l) const noexcept
	{
		std::size_t seed = 0;

		auto hash_combine = [&seed](std::size_t h)
			{
				seed ^= h + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			};

		hash_combine(std::hash<std::string>{}(l.type_));
		hash_combine(std::hash<std::string>{}(l.name_));
		hash_combine(std::hash<float>{}(l.position_.x));
		hash_combine(std::hash<float>{}(l.position_.y));
		hash_combine(std::hash<bool>{}(l.visible_));
		hash_combine(std::hash<float>{}(l.opacity_));

		for (const int val : l.data_)
			hash_combine(std::hash<int>{}(val));

		return seed;
	}
};

inline void to_json(json& j, const LayerBlueprint& lb)
{
	j =
	{
		{"x", lb.position_.x},
		{"y", lb.position_.y},
		{"name", lb.name_},
		{"opacity", lb.opacity_},
		{"visible", lb.visible_},
		{"data", lb.data_},
		{"type", lb.type_}
	};
}

inline void from_json(const json& j, LayerBlueprint& lb)
{
	j.at("x").get_to(lb.position_.x);
	j.at("y").get_to(lb.position_.y);
	j.at("type").get_to(lb.type_);
	j.at("name").get_to(lb.name_);
	j.at("opacity").get_to(lb.opacity_);
	j.at("visible").get_to(lb.visible_);
	j.at("data").get_to(lb.data_);
}

}
