#pragma once

#include <nlohmann/json.hpp>
#include <NiEngine/PolygonBlueprint.h>

struct LevelStartBlueprint
{
	int  start_level_ = 0;
	int  total_level_count_ = 0;
	bool skip_intro_  = false;
};

inline void to_json(json& j, const LevelStartBlueprint& tb)
{
	j =
	{
		{"total_level_count", tb.total_level_count_},
		{"start_level", tb.start_level_},
		{"skip_intro" , tb.skip_intro_ },
	};
}

inline void from_json(const json& j, LevelStartBlueprint& tb)
{
	j.at("start_level").get_to(tb.start_level_);
	j.at("total_level_count").get_to(tb.total_level_count_);
	j.at("skip_intro" ).get_to(tb.skip_intro_);
}