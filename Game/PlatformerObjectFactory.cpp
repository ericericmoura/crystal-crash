#include "PlatformerObjectFactory.h"

#include <vector>
#include <string>
#include <memory>
#include <utility>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <NiEngine/TilesetBlueprint.h>
#include <NiEngine/GameMode.h>
#include <NiEngine/ObjectTemplateBlueprint.h>
#include <NiEngine/ObjectBlueprint.h>
#include <NiEngine/AnimatedGraphicsComponent.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/TransformComponent.h>
#include <NiEngine/DataHandler.h>
#include <NiEngine/TiledUtility.h>
#include <NiEngine/FileUtility.h>

void PlatformerObjectFactory::SpawnObject(ni::ObjectBlueprint object, ni::ObjectTemplateBlueprint& object_template, const std::vector<ni::TilesetBlueprint>& tileset_blueprints, ni::GameMode& mode, int type)
{
	ni::DataHandler<ni::TilesetBlueprint> tileset_handler(ni::FileUtility::RemoveRelativePaths(object_template.tileset_reference_.file_source_));
	ni::TilesetBlueprint tileset = tileset_handler.GetBlueprint();

	std::string texture_key = tileset.texture_key_;
	
	int gid  = object_template.tile_gid_ - tileset.first_gid_;

	int uv_x = gid % tileset.columns_;
	int uv_y = gid / tileset.columns_;
	
	sf::IntRect texture_coords;
	texture_coords.position.x = uv_x * (tileset.tile_size_.x + tileset.spacing_);
	texture_coords.position.y = uv_y * (tileset.tile_size_.y + tileset.spacing_);
	texture_coords.size = tileset.tile_size_;

	switch (type)
	{
	case ObjectTypes::Slingshot:
		SpawnSlingshot(object, object_template, texture_key, texture_coords, mode);
		break;
	}
}

void PlatformerObjectFactory::SpawnSlingshot(ni::ObjectBlueprint object, ni::ObjectTemplateBlueprint& object_template, std::string texture_key, sf::IntRect texture_coordinates, ni::GameMode& mode)
{
	ni::Id<ni::GameObjectTag> id = mode.CreateGameObject();

	object.position_.x += texture_coordinates.size.x / 2.0f;
	object.position_.y += texture_coordinates.size.y / 2.0f;

	auto graphics = std::make_unique<ni::AnimatedGraphicsComponent>(texture_key, texture_coordinates.size, 1);

	ni::TransformComponent transform;
	transform.GetTransformable().setPosition(object.position_);
	transform.GetTransformable().setOrigin({ texture_coordinates.size.x / 2.0f, texture_coordinates.size.y / 2.0f });

	mode.GetComponentStore().AttachGraphicsComponent(id, std::move(graphics));
	mode.GetComponentStore().AttachTransformComponent(id, transform);
}
