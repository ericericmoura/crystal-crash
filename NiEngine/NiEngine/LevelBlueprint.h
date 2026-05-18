#pragma once

#include <vector>

#include <SFML/System/Vector2.hpp>
#include <nlohmann/json.hpp>

#include <NiEngine/LayerBlueprint.h>
#include <NiEngine/TilesetReference.h>

#include "ObjectLayerBlueprint.h"

using json = nlohmann::json;

namespace ni {

struct LevelBlueprint
{
	sf::Vector2i tile_size_ = {};
	sf::Vector2i map_size_ = {};

	std::vector<TilesetReference> tileset_references_ = {};
	
	std::vector<LayerBlueprint>	      layers_        = {};
	std::vector<ObjectLayerBlueprint> object_layers_ = {};
};

inline void to_json(json& j, const LevelBlueprint& tb)
{
	j =
	{
		{"height",	   tb.map_size_.y},
		{"width",      tb.map_size_.x},
		{"tileWidth",  tb.tile_size_.x },
		{"tileHeight", tb.tile_size_.y },
		{"tilesets",   tb.tileset_references_ },
		{"layers",     tb.layers_ },
	};	
}

inline void from_json(const json& j, LevelBlueprint& tb)
{
	std::string dump = j.dump(4);

	j.at("height").get_to(tb.map_size_.y);
	j.at("width").get_to(tb.map_size_.x);

	j.at("tilewidth").get_to(tb.tile_size_.x);
	j.at("tileheight").get_to(tb.tile_size_.y);

	j.at("tilesets").get_to(tb.tileset_references_);

	for (auto& layer : j.at("layers"))
	{
		if (layer.contains("data"))
		{
			tb.layers_.push_back(layer);
			continue;
		}
		if (layer.contains("objects"))
		{
			tb.object_layers_.push_back(layer);
		}
	}
}

}