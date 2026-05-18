#include <NiEngine/TilemapCollisionComponent.h>

#include <box2d.h>
#include <types.h>
#include <math_functions.h>
#include <id.h>

#include <unordered_map>
#include <vector>

#include <SFML/System/Vector2.hpp>
#include <NiEngine/Converter.h>
#include <NiEngine/LayerBlueprint.h>
#include <NiEngine/TilesetBlueprint.h>
#include <NiEngine/TileBlueprint.h>
#include <NiEngine/Tilemap.h>
#include <NiEngine/CollisionBits.h>
#include <NiEngine/TilemapUtility.h>

std::vector<ni::LoopInformation> ni::TilemapCollisionComponent::GetCollisionLoops(EdgesMap& map)
{
	std::vector<LoopInformation> loops;

	while (!map.empty())
	{
		sf::Vector2i starting_point = map.begin()->first;
		sf::Vector2i current_point  = starting_point;

		bool is_closed = true;
		std::vector<b2Vec2> loop;
		do
		{
			loop.push_back(Converter::PixelsToMeters(sf::Vector2i({ current_point.x, current_point.y })));

			auto it = map.find(current_point);
			if (it == map.end())
			{
				is_closed = false;
				break;
			}

			sf::Vector2i nextPoint = it->second;

			map.erase(current_point);
			current_point = nextPoint;

		} while (current_point != starting_point);

		loops.push_back({ is_closed, loop });
	}

	return loops;
}

void ni::TilemapCollisionComponent::AddFullCollisionForTile(
	const LayerBlueprint& layer,
	sf::Vector2i tile_grid_position,
	sf::Vector2i tile_position,
	sf::Vector2i map_size,
	sf::Vector2i tile_size) 
{

	struct EdgeDef 
	{
		sf::Vector2i start_offset;
		sf::Vector2i end_offset;
		sf::Vector2i neighbor_offset;
	};

	const EdgeDef kEdges[] = 
	{
		{ {0,            0           }, {tile_size.x, 0           }, {0,  -1} }, // Top
		{ {tile_size.x,  tile_size.y }, {0,			  tile_size.y }, {0,  +1} }, // Bottom
		{ {0,            tile_size.y }, {0,           0           }, {-1,  0} }, // Left
		{ {tile_size.x,  0           }, {tile_size.x, tile_size.y }, {+1,  0} }, // Right
	};

	for (const auto& edge : kEdges) 
	{
		sf::Vector2i neighbor = tile_grid_position + edge.neighbor_offset;
		if (!TilemapUtility::IsTileEmpty(layer.data_, map_size, neighbor)) continue;

		sf::Vector2i start = tile_position + edge.start_offset;
		sf::Vector2i end = tile_position + edge.end_offset;
		exposed_edges_[start] = end;
	}
}

void ni::TilemapCollisionComponent::CreateOnesidedCollision()
{
	std::vector<LoopInformation> one_sided_loops = GetCollisionLoops(one_sided_edges_);

	for (const auto& loop : one_sided_loops)
	{
		std::vector<b2Vec2> padded;
		padded.reserve(loop.data_.size() + 2);

		b2Vec2 ghost_start = {
			loop.data_[0].x - (loop.data_[0].x - loop.data_[1].x),
			loop.data_[0].y - (loop.data_[0].y - loop.data_[1].y)
		};
		padded.push_back(ghost_start);

		for (const auto& p : loop.data_)
			padded.push_back(p);

		b2Vec2 ghost_end = {
			loop.data_.back().x - (loop.data_[loop.data_.size() - 2].x - loop.data_.back().x),
			loop.data_.back().y - (loop.data_[loop.data_.size() - 2].y - loop.data_.back().y)
		};
		padded.push_back(ghost_end);

		b2ChainDef shape = b2DefaultChainDef();
		shape.filter = { StaticBit, AllBits, 0 };
		shape.points = padded.data();
		shape.count = padded.size();
		shape.isLoop = false;
		b2CreateChain(body_id_, &shape);
	}
}

ni::TilemapCollisionComponent::TilemapCollisionComponent(b2WorldId world_id)
{
	b2BodyDef body_def = b2DefaultBodyDef();
	body_def.type = b2_staticBody;

	body_id_ = b2CreateBody(world_id, &body_def);
}

void ni::TilemapCollisionComponent::AddTile(
	sf::Vector2i grid_position, 
	int tile_gid, 
	const TilesetBlueprint& tileset, 
	const LayerBlueprint& layer, 
	sf::Vector2i map_size, 
	sf::Vector2i tile_size)
{
	int gx = grid_position.x;
	int gy = grid_position.y;

	int x = grid_position.x * tile_size.x;
	int y = grid_position.y * tile_size.y;

	auto it = tileset.tiles_.find(tile_gid);
	if (it == tileset.tiles_.end() || (!it->second.is_hill_ && !it->second.one_sided_collision_))
	{
		AddFullCollisionForTile(layer, grid_position, { x, y }, map_size, tile_size);
		return;
	}
		
	const TileBlueprint& tile = tileset.tiles_.at(tile_gid);

	// One sided (above only) collision
	bool is_top_empty = TilemapUtility::IsTileEmpty(layer.data_, map_size, { gx, gy - 1 });
	if (is_top_empty && tile.one_sided_collision_)
	{
		one_sided_edges_[{x, y}] = { x + tile_size.x, y };
		return;
	}

	// Hill Collision
	if (tile.is_hill_)
	{
		sf::Vector2i target_vertice = tile.polygon_blueprint_.position_;
			
		sf::Vector2i top_left = target_vertice + tile.polygon_blueprint_.offset_points_.at(0);
		top_left.x += x;
		top_left.y += y;

		sf::Vector2i top_right = target_vertice + tile.polygon_blueprint_.offset_points_.at(1);
		top_right.x += x;
		top_right.y += y;

		exposed_edges_[top_left] = top_right;
		return;
	}
}

void ni::TilemapCollisionComponent::CreateCollision()
{	
	std::vector<LoopInformation> loops = GetCollisionLoops(exposed_edges_);

	for (const auto& loop : loops)
	{
		b2ChainDef shape = b2DefaultChainDef();
		shape.filter = { StaticBit, AllBits, 0 };
		shape.points = loop.data_.data();
		shape.count  = loop.data_.size();
		shape.isLoop = loop.is_closed_;

		b2CreateChain(body_id_, &shape);
	}

	CreateOnesidedCollision();
}
