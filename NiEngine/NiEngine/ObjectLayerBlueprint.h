#pragma once

#include <string>
#include <vector>

#include <SFML/System/Vector2.hpp>
#include <nlohmann/json.hpp>

#include "ObjectBlueprint.h"

using json = nlohmann::json;

namespace ni {

struct ObjectLayerBlueprint
{
	std::string  name_ = "";
	sf::Vector2f position_ = {};

	std::vector<ObjectBlueprint> objects_ = {};
};

inline void to_json(json& j, const ObjectLayerBlueprint& lb)
{
	j =
	{
		{"x",       lb.position_.x},
		{"y",       lb.position_.y},
		{"name",    lb.name_},
		{"objects", lb.objects_}
	};
}

inline void from_json(const json& j, ObjectLayerBlueprint& lb)
{
	j.at("x")      .get_to(lb.position_.x);
	j.at("y")      .get_to(lb.position_.y);
	j.at("name")   .get_to(lb.name_);
	j.at("objects").get_to(lb.objects_);
}

}