#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <NiEngine/TilesetBlueprint.h>
#include <NiEngine/BitmapStore.h>

namespace ni {

struct TilesetDrawable
{
	sf::VertexArray vertices_ = {};
	sf::Vector2f	position_offset_ = {};
};

class TilemapGraphicsComponent
{
private:
	std::unordered_map<std::string, TilesetDrawable> tileset_drawables_;

public:
	void Clear()
	{
		for (auto& [key, drawable] : tileset_drawables_)
		{
			drawable.vertices_.clear();
		}
	}

	void AddTile(const sf::Vector2i& grid_position, int tile_id, const TilesetBlueprint& tileset, const sf::Vector2f& layer_position_offset);
	void Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store);

	sf::FloatRect GetBounds(sf::Vector2i map_size, sf::Vector2i tile_size) const;
};

}