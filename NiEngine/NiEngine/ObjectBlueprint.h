#pragma once
#pragma once

#include <string>
#include <unordered_map>

#include <SFML/System/Vector2.hpp>
#include <nlohmann/json.hpp>

#include "PropertyBlueprint.h"

using json = nlohmann::json;

namespace ni {

struct ObjectBlueprint
{
	int id_ = 0;
	std::string  template_filepath_ = "";
	sf::Vector2f position_ = {};

	std::unordered_map<std::string, PropertyBlueprint> properties_;
};

inline void to_json(json& j, const ObjectBlueprint& lb)
{
	j =
	{
		{"id",		   lb.id_},
		{"x",		   lb.position_.x},
		{"y",		   lb.position_.y},
		{"template",   lb.template_filepath_}
	};

	for (auto& [key, value] : lb.properties_)
	{
		j["properties"] += value;
	}

}

inline void from_json(const json& j, ObjectBlueprint& lb)
{
	j.at("id")        .get_to(lb.id_);
	j.at("x")         .get_to(lb.position_.x);
	j.at("y")         .get_to(lb.position_.y);
	j.at("template")  .get_to(lb.template_filepath_);
	
	if (!j.contains("properties"))
	{
		return;	
	}

	for (auto& property : j.at("properties"))
	{
		lb.properties_[property.at("name")] = property;
	}
}

}
