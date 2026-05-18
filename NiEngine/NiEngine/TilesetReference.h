#pragma once

#include <string>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace ni {

struct TilesetReference
{
	int first_gid_ = 0;
	std::string file_source_ = "";
};

inline void to_json(json& j, const TilesetReference& tr)
{
	j =
	{
		{"firstgid", tr.first_gid_},
		{"source",   tr.file_source_},
	};
}

inline void from_json(const json& j, TilesetReference& tr)
{
	j.at("firstgid").get_to(tr.first_gid_);
	j.at("source").get_to(tr.file_source_);
}

}