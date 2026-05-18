#pragma once

#include <unordered_set>
#include <vector>

#include "TilesetBlueprint.h"

namespace ni {

struct TiledUtility
{
	static inline ni::TilesetBlueprint GetTilesetByGid(const std::vector<TilesetBlueprint>& tileset_blueprints, int gid)
	{
		for (auto& tileset : tileset_blueprints)
		{
			if (gid >= tileset.first_gid_)
			{
				return tileset;
			}
		}
		return {};
	}

	static inline ni::TilesetBlueprint GetTilesetByGid(const std::unordered_set<TilesetBlueprint, TilesetBlueprintHash>& tileset_blueprints, int gid)
	{
		std::vector<TilesetBlueprint> v;
		v.assign(tileset_blueprints.begin(), tileset_blueprints.end());

		return GetTilesetByGid(v, gid);
	}
};

}