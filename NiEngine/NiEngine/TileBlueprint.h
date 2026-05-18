#pragma once

#include <type_traits>

#include <nlohmann/json.hpp>
#include <NiEngine/PolygonBlueprint.h>

using json = nlohmann::json;

namespace ni {

struct TileBlueprint
{
	int id_ = 0;

	bool is_hill_ = false;
	bool one_sided_collision_ = false;

	PolygonBlueprint polygon_blueprint_;

	inline bool operator==(const TileBlueprint& b) const
	{
		return id_ == b.id_
			&& is_hill_ == b.is_hill_
			&& one_sided_collision_ == b.one_sided_collision_
			&& polygon_blueprint_   == b.polygon_blueprint_;
	}
};

struct TileBlueprintHash
{
	std::size_t operator()(const TileBlueprint& t) const noexcept
	{
		std::size_t seed = 0;

		auto hash_combine = [&seed](std::size_t h)
			{
				seed ^= h + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			};

		hash_combine(std::hash<int>{}(t.id_));
		hash_combine(std::hash<bool>{}(t.is_hill_));
		hash_combine(std::hash<bool>{}(t.one_sided_collision_));
		hash_combine(PolygonBlueprintHash{}(t.polygon_blueprint_));

		return seed;
	}
};

inline void to_json(json& j, const TileBlueprint& tb)
{
	j =
	{
		{"id", tb.id_}
	};

	j["properties"] += 
	{ 
		{"name", "oneSidedCollision"},
		{"value"}, tb.one_sided_collision_
	};

	j["properties"] +=
	{
		{"name", "isHill"},
		{ "value" }, tb.is_hill_
	};	

	if (tb.polygon_blueprint_.id_ > 0)
	{
		j["objectgroup"]["objects"].push_back(tb.polygon_blueprint_);
	}
}

inline void from_json(const json& j, TileBlueprint& tb)
{
	j.at("id").get_to(tb.id_);

	if (j.contains("objectgroup") && j.at("objectgroup").contains("objects") && j.at("objectgroup").at("objects").size() > 0)
	{
		for (auto& polygon_json : j.at("objectgroup").at("objects"))
		{
			tb.polygon_blueprint_ = polygon_json;
		}
	}

	if (!j.contains("properties"))
	{
		return;
	}

	for (auto& property : j.at("properties"))
	{
		if (property.at("name") == "oneSidedCollision")
		{
			property.at("value").get_to(tb.one_sided_collision_);
		}
		else if (property.at("name") == "isHill")
		{
			property.at("value").get_to(tb.is_hill_);
		}
	}
}

}