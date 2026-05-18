#include "PlatformerObjectFactory.h"

#include <vector>
#include <memory>
#include <utility>

#include <NiEngine/TilesetBlueprint.h>
#include <NiEngine/GameMode.h>
#include <NiEngine/ObjectTemplateBlueprint.h>
#include <NiEngine/ObjectBlueprint.h>
#include <NiEngine/AnimatedGraphicsComponent.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/TransformComponent.h>
#include <NiEngine/DataHandler.h>
#include <NiEngine/FileUtility.h>
#include <NiEngine/TileBlueprint.h>

#include "SlingshotUpdateComponent.h"

void PlatformerObjectFactory::SpawnObject(ni::ObjectBlueprint object, ni::ObjectTemplateBlueprint& object_template, const std::vector<ni::TilesetBlueprint>& tileset_blueprints, ni::GameMode& mode, int type)
{
	ni::DataHandler<ni::TilesetBlueprint> tileset_handler(ni::FileUtility::RemoveRelativePaths(object_template.tileset_reference_.file_source_));
	ni::TilesetBlueprint tileset = tileset_handler.GetBlueprint();

	int gid  = object_template.tile_gid_ - tileset.first_gid_ - 1;

	ni::TileBlueprint& tile = tileset.tiles_.at(gid);

	switch (type)
	{
	case ObjectTypes::Slingshot:
		SpawnSlingshot(object, object_template, tile, mode);
		break;
	}
}

void PlatformerObjectFactory::SpawnSlingshot(ni::ObjectBlueprint object, ni::ObjectTemplateBlueprint& object_template, ni::TileBlueprint& tile_blueprint, ni::GameMode& mode)
{
	ni::Id<ni::GameObjectTag> id = mode.CreateGameObject();

	object.position_.y -= tile_blueprint.image_size_.y;

	auto update = std::make_unique<SlingshotUpdateComponent>(mode.GetComponentStore());

	auto graphics = std::make_unique<ni::AnimatedGraphicsComponent>(ni::FileUtility::RemoveRelativePaths(tile_blueprint.image_key_), tile_blueprint.image_size_, 1);

	ni::TransformComponent transform;
	transform.GetTransformable().setPosition(object.position_);

	mode.GetComponentStore().AttachUpdateComponent   (id, std::move(update));
	mode.GetComponentStore().AttachGraphicsComponent (id, std::move(graphics));
	mode.GetComponentStore().AttachTransformComponent(id, transform);
}
