#include <NiEngine/Tilemap.h>

#include <id.h>

#include <string>
#include <vector>
#include <memory>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include <NiEngine/TilesetBlueprint.h>
#include <NiEngine/LayerBlueprint.h>
#include <NiEngine/BitmapStore.h>
#include <NiEngine/TileBlueprint.h>
#include <NiEngine/TilemapCollisionComponent.h>
#include <NiEngine/TiledUtility.h>
#include <NiEngine/Level.h>

void ni::Tilemap::Clear()
{
	graphics_.Clear();
	layers_.clear();

	if (collision_)
	{
		collision_->Clear();
	}
}

void ni::Tilemap::Init(sf::Vector2i map_size, sf::Vector2i tile_size)
{
	map_size_  = map_size;
	tile_size_ = tile_size;
}

void ni::Tilemap::LoadTiles(LayerBlueprint layer_blueprint, const std::vector<TilesetBlueprint>& tileset_blueprints)
{
	layers_.emplace(layer_blueprint);
	for (int y = 0; y < map_size_.y; ++y)
	{
		for (int x = 0; x < map_size_.x; ++x)
		{
			int index = x + y * map_size_.x;
			int tile_id = layer_blueprint.data_.at(index);
			if (tile_id == 0)
			{
				continue;
			}

			const TilesetBlueprint& tileset_blueprint = TiledUtility::GetTilesetByGid(tileset_blueprints, tile_id);
			tilesets_.emplace(tileset_blueprint);

			tile_id -= tileset_blueprint.first_gid_;

			graphics_.AddTile({ x, y }, tile_id, tileset_blueprint, layer_blueprint.position_);

			if (collision_)
			{
				collision_->AddTile({ x, y }, tile_id, tileset_blueprint, layer_blueprint, map_size_, tile_size_);
			}
		}
	}
	if (collision_)
	{
		collision_->CreateCollision();
	}
}

void ni::Tilemap::EnableCollision(b2WorldId world_id)
{
	collision_ = std::make_unique<TilemapCollisionComponent>(world_id);
}

void ni::Tilemap::Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store)
{
	graphics_.Render(target, states, store);
}

sf::FloatRect ni::Tilemap::GetBounds() const
{
	return graphics_.GetBounds(map_size_, tile_size_);
}

sf::Vector2i ni::Tilemap::GlobalToGridPosition(sf::Vector2f position) const
{
	sf::Vector2i result;
	result.x = position.x / tile_size_.x;
	result.y = position.y / tile_size_.y;
	return result;
}

ni::TileBlueprint ni::Tilemap::GetTileInfo(sf::Vector2i tile_grid_position, const std::string& layer_name) const
{
	bool out_of_bounds = tile_grid_position.x < 0 || tile_grid_position.x >= map_size_.x ||
						 tile_grid_position.y < 0 || tile_grid_position.y >= map_size_.y;
	if (out_of_bounds)
	{
		return TileBlueprint();
	}

	LayerBlueprint layer = GetLayerByName(layer_name);
	if (layer.name_ == "")
	{
		return TileBlueprint();
	}

	int tile_index = tile_grid_position.x + tile_grid_position.y * map_size_.x;
	if (tile_index > 0 && tile_index < (int)layer.data_.size() && layer.data_[tile_index] != 0)
	{
		int tile_gid = layer.data_[tile_index];
		TilesetBlueprint tileset = TiledUtility::GetTilesetByGid(tilesets_, tile_gid);

		auto it = tileset.tiles_.find(tile_gid - tileset.first_gid_);
		if (it != tileset.tiles_.end())
		{
			return tileset.tiles_.at(tile_gid - tileset.first_gid_);
		}
	}

	return TileBlueprint();
}

ni::LayerBlueprint ni::Tilemap::GetLayerByName(const std::string& layer_name) const
{
	for (auto& layer : layers_)
	{
		if (layer.name_ == layer_name)
		{
			return layer;
		}
	}
	return {};
}

bool ni::Tilemap::IsTileEmpty(sf::Vector2i tile_grid_position) const
{
	bool out_of_bounds = tile_grid_position.x < 0 || tile_grid_position.x >= map_size_.x ||
						 tile_grid_position.y < 0 || tile_grid_position.y >= map_size_.y;

	if (out_of_bounds)
	{
		return true;
	}

	for (auto& layer : layers_)
	{
		if (layer.name_ == Level::kPrototypeLayerName || layer.name_ == Level::kIgnoreLayerName || layer.type_ == Level::kObjectsLayerType)
		{
			continue;
		}

		int tile_index = tile_grid_position.x + tile_grid_position.y * map_size_.x;

		if (tile_index > 0 && tile_index < (int)layer.data_.size() && layer.data_[tile_index] != 0)
		{
			return false;
		}
	}
	return true;
}
