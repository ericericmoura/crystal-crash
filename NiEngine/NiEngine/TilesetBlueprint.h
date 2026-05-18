#pragma once

#include <string>
#include <unordered_map>
#include <algorithm>
#include <type_traits>
#include <vector>

#include <SFML/System/Vector2.hpp>
#include <nlohmann/json.hpp>

#include <NiEngine/MapUtility.h>
#include <NiEngine/TileBlueprint.h>

using json = nlohmann::json;

namespace ni {

struct TilesetBlueprint
{
	std::string name_ = "";
	std::string texture_key_ = "";

	sf::Vector2i tile_size_  = {};
	sf::Vector2i image_size_ = {};

	int spacing_	= 0;
	int margin_		= 0;
	int columns_	= 0;
	int tile_count_ = 0;

	std::unordered_map<int, TileBlueprint> tiles_ = {};

	int first_gid_ = 0;

	inline bool operator==(const TilesetBlueprint& b) const
	{
		return name_ == b.name_
			&& texture_key_ == b.texture_key_
			&& tile_size_.x == b.tile_size_.x
			&& tile_size_.y == b.tile_size_.y
			&& image_size_.x == b.image_size_.x
			&& image_size_.y == b.image_size_.y
			&& spacing_ == b.spacing_
			&& margin_ == b.margin_
			&& columns_ == b.columns_
			&& tile_count_ == b.tile_count_
			&& first_gid_ == b.first_gid_
			&& tiles_ == b.tiles_;
	}
};

struct TilesetBlueprintHash
{
	std::size_t operator()(const TilesetBlueprint& t) const noexcept
	{
		std::size_t seed = 0;

		auto hash_combine = [&seed](std::size_t h)
			{
				seed ^= h + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			};

		hash_combine(std::hash<std::string>{}(t.name_));
		hash_combine(std::hash<std::string>{}(t.texture_key_));
		hash_combine(std::hash<int>{}(t.tile_size_.x));
		hash_combine(std::hash<int>{}(t.tile_size_.y));
		hash_combine(std::hash<int>{}(t.image_size_.x));
		hash_combine(std::hash<int>{}(t.image_size_.y));
		hash_combine(std::hash<int>{}(t.spacing_));
		hash_combine(std::hash<int>{}(t.margin_));
		hash_combine(std::hash<int>{}(t.columns_));
		hash_combine(std::hash<int>{}(t.tile_count_));
		hash_combine(std::hash<int>{}(t.first_gid_));

		std::vector<int> keys;
		keys.reserve(t.tiles_.size());
		for (const auto& [key, _] : t.tiles_)
			keys.push_back(key);
		std::sort(keys.begin(), keys.end());

		for (const int key : keys)
		{
			hash_combine(std::hash<int>{}(key));
			hash_combine(TileBlueprintHash{}(t.tiles_.at(key)));
		}
		return seed;
	}
};

inline void to_json(json& j, const TilesetBlueprint& tb)
{
	j =
	{
		{"name",	   tb.name_},
		{"image",      tb.texture_key_},
		{"tilewidth",  tb.tile_size_.x },
		{"tileheight", tb.tile_size_.y },
		{"imagewidth",  tb.image_size_.x },
		{"imageheight", tb.image_size_.y },
		{"spacing",    tb.spacing_ },
		{"margin",     tb.margin_ },
		{"columns",    tb.columns_},
		{"tilecount",  tb.tile_count_ },
		{"tiles",      MapUtility::MapToVector<int, TileBlueprint>(tb.tiles_)}
	};
}

inline void from_json(const json& j, TilesetBlueprint& tb)
{
	j.at("name").get_to(tb.name_);
	if (j.contains("image"))
	{
		j.at("image").get_to(tb.texture_key_);
		j.at("imagewidth").get_to(tb.image_size_.x);
		j.at("imageheight").get_to(tb.image_size_.y);
	}
	j.at("tilewidth").get_to(tb.tile_size_.x);
	j.at("tileheight").get_to(tb.tile_size_.y);
	j.at("spacing").get_to(tb.spacing_);
	j.at("margin").get_to(tb.margin_);
	j.at("columns").get_to(tb.columns_);
	j.at("tilecount").get_to(tb.tile_count_);

	if (!j.contains("tiles"))
	{
		return;
	}

	auto& tiles_json = j.at("tiles");
	for (const auto& tile : tiles_json)
	{
		TileBlueprint tile_blueprint = tile;
		tb.tiles_[tile_blueprint.id_] = tile_blueprint;
	}
}

}