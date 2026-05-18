#pragma once

#include <id.h>

#include <string>
#include <memory>
#include <vector>
#include <unordered_set>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include "TilemapGraphicsComponent.h"
#include "LayerBlueprint.h"
#include "BitmapStore.h"
#include "TilemapCollisionComponent.h"
#include "TileBlueprint.h"
#include "TilesetBlueprint.h"

namespace ni {

class Tilemap
{
private:
	std::unordered_set<LayerBlueprint,   LayerBlueprintHash>   layers_;
	std::unordered_set<TilesetBlueprint, TilesetBlueprintHash> tilesets_;

	TilemapGraphicsComponent   graphics_;

	std::unique_ptr<TilemapCollisionComponent> collision_ = nullptr;

	bool has_loaded_collision = false;
	bool has_loaded_graphics  = false;

	bool is_collision_enabled = false;

	sf::Vector2i map_size_  = {};
	sf::Vector2i tile_size_ = {};

public:
	void Clear();

	void Init(sf::Vector2i map_size, sf::Vector2i tile_size);
	void LoadTiles(LayerBlueprint layer, const std::vector<TilesetBlueprint>& tileset_blueprints);
	void EnableCollision(b2WorldId world_id);
	void Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store);

	sf::FloatRect GetBounds() const;

	sf::Vector2i GlobalToGridPosition(sf::Vector2f position) const;

	TileBlueprint  GetTileInfo(sf::Vector2i tile_grid_position, const std::string& layer_name) const;
	LayerBlueprint GetLayerByName(const std::string& layer_name) const;

	bool IsTileEmpty(sf::Vector2i tile_grid_position) const;
};

}