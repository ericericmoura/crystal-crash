#pragma once

#include <string>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "TilesetReference.h"
#include "PropertyBlueprint.h"

using json = nlohmann::json;

namespace ni {

struct ObjectTemplateBlueprint
{
	int id_ = 0;
	int tile_gid_ = 0;
	int width_ = 16;
	std::string name_ = "";

	std::unordered_map<std::string, PropertyBlueprint> properties_map_;

	TilesetReference tileset_reference_ = {};
};

inline void to_json(json& j, const ObjectTemplateBlueprint& lb)
{
	j["object"] = {
		{ "name", lb.name_ },
		{ "id", lb.id_ },
		{ "gid", lb.tile_gid_ },
		{ "width", lb.width_ }
	};

	for (auto& [key, value] : lb.properties_map_)
	{
		j["properties"] += value;
	}

	j =
	{
		{"tileset", lb.tileset_reference_}
	};
}

inline void from_json(const json& j, ObjectTemplateBlueprint& lb)
{
	j.at("object").at("name")   .get_to(lb.name_);
	j.at("object").at("id")     .get_to(lb.id_);
	j.at("object").at("gid")    .get_to(lb.tile_gid_);
	j.at("object").at("width")  .get_to(lb.width_);
	j.at("tileset").get_to(lb.tileset_reference_);

	for (auto& property : j.at("object").at("properties"))
	{
		lb.properties_map_[property.at("name")] = property;
	}
}

}