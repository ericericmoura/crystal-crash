#pragma once

#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

namespace ni {

struct TilemapUtility 
{
	static sf::IntRect GetTextureBounds(sf::Vector2i coordinates, int tile_size = 0, int spacing = 0)
	{
		sf::IntRect result;
		result.size = { tile_size, tile_size };
		result.position.x = coordinates.x * (tile_size + spacing);
		result.position.y = coordinates.y * (tile_size + spacing);
		return result;
	}

	static bool IsTileEmpty(const std::vector<int>& map, sf::Vector2i map_size, sf::Vector2i tile_grid_position)
	{
		bool is_out_of_bounds = tile_grid_position.x < 0 || tile_grid_position.x >= map_size.x ||
			tile_grid_position.y < 0 || tile_grid_position.y >= map_size.y;
		if (is_out_of_bounds)
		{
			return true;
		}
		int tile_index = tile_grid_position.x + tile_grid_position.y * map_size.x;

		return tile_index < 0 || tile_index >= (int)map.size() || map[tile_index] == 0;
	}
};

}
