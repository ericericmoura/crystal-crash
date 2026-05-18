#pragma once

#include <id.h>
#include <math_functions.h>

#include <vector>
#include <unordered_map>

#include <SFML/System/Vector2.hpp>

#include "LayerBlueprint.h"
#include "Vector2iHash.h"
#include "TilesetBlueprint.h"

namespace ni {

struct LoopInformation
{
	bool is_closed_ = false;
	std::vector<b2Vec2> data_;
};

using EdgesMap = std::unordered_map<sf::Vector2i, sf::Vector2i, Vector2iHash>;

class TilemapCollisionComponent
{
private:
	b2BodyId body_id_ = {};

	EdgesMap exposed_edges_   = {};
	EdgesMap one_sided_edges_ = {};

	std::vector<LoopInformation> GetCollisionLoops(EdgesMap& map);

	void AddFullCollisionForTile(const LayerBlueprint& layer, sf::Vector2i tile_grid_position, sf::Vector2i tile_position, sf::Vector2i map_size, sf::Vector2i tile_size);

	void CreateOnesidedCollision();

public:
	TilemapCollisionComponent(b2WorldId world_id);

	void Clear()
	{
		exposed_edges_ = {};
		one_sided_edges_ = {};
		body_id_ = {};
	}

	void AddTile(sf::Vector2i grid_position, int tile_gid, const TilesetBlueprint& tileset, const LayerBlueprint& layer, sf::Vector2i map_size, sf::Vector2i tile_size);
	void CreateCollision();
};

}