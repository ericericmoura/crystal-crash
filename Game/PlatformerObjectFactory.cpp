#include "PlatformerObjectFactory.h"

#include <vector>
#include <string>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <NiEngine/TilesetBlueprint.h>
#include <NiEngine/GameMode.h>
#include <NiEngine/ObjectTemplateBlueprint.h>
#include <NiEngine/ObjectBlueprint.h>
#include <NiEngine/TiledUtility.h>

void PlatformerObjectFactory::SpawnObject(ni::ObjectBlueprint object, ni::ObjectTemplateBlueprint& object_template, const std::vector<ni::TilesetBlueprint>& tileset_blueprints, ni::GameMode& mode, int type)
{
	ni::TilesetBlueprint tileset = ni::TiledUtility::GetTilesetByGid(tileset_blueprints, object_template.tile_gid_);
	std::string  texture_key = tileset.texture_key_;
	
	int gid  = object_template.tile_gid_ - tileset.first_gid_;

	int uv_x = gid % tileset.columns_;
	int uv_y = gid / tileset.columns_;
	
	sf::IntRect texture_coords;
	texture_coords.position.x = uv_x * (tileset.tile_size_.x + tileset.spacing_);
	texture_coords.position.y = uv_y * (tileset.tile_size_.y + tileset.spacing_);
	texture_coords.size = tileset.tile_size_;

	switch (type)
	{
	default:
		break;
	}
}