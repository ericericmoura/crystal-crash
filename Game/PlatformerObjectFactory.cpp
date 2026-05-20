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
#include <NiEngine/Animation.h>

#include "PlatformerGameMode.h"
#include "SlingshotUpdateComponent.h"

void PlatformerObjectFactory::SpawnObject(ni::ObjectBlueprint object, ni::ObjectTemplateBlueprint& object_template, const std::vector<ni::TilesetBlueprint>& tileset_blueprints, ni::GameMode& mode, int type)
{
	ni::DataHandler<ni::TilesetBlueprint> tileset_handler(ni::FileUtility::RemoveRelativePaths(object_template.tileset_reference_.file_source_));
	
	ni::TilesetBlueprint tileset = tileset_handler.GetBlueprint();
	switch (type)
	{
	case ObjectTypes::Slingshot:
		SpawnSlingshot(object, object_template, tileset, mode);
		break;
	case ObjectTypes::Ammo:
		SpawnAmmo(object, object_template, tileset, mode);
		break;
	}
}

void PlatformerObjectFactory::SpawnSlingshot(ni::ObjectBlueprint object, ni::ObjectTemplateBlueprint& object_template, ni::TilesetBlueprint& tileset, ni::GameMode& mode)
{
	int gid = object_template.tile_gid_ - tileset.first_gid_ - 1;

	ni::TileBlueprint& tile = tileset.tiles_.at(gid);

	ni::Id<ni::GameObjectTag> id = mode.CreateGameObject();

	object.position_.y -= tile.image_size_.y;

	auto update   = std::make_unique<SlingshotUpdateComponent>(mode.GetComponentStore(), object.position_, tile.image_size_);
	auto graphics = std::make_unique<ni::AnimatedGraphicsComponent>(ni::FileUtility::RemoveRelativePaths(tile.image_key_), tile.image_size_, 1);

	ni::TransformComponent transform;
	transform.GetTransformable().setPosition(object.position_);

	mode.GetComponentStore().AttachUpdateComponent   (id, std::move(update));
	mode.GetComponentStore().AttachGraphicsComponent (id, std::move(graphics));
	mode.GetComponentStore().AttachTransformComponent(id, transform);
}

void PlatformerObjectFactory::SpawnAmmo(ni::ObjectBlueprint object, ni::ObjectTemplateBlueprint& object_template, ni::TilesetBlueprint& tileset, ni::GameMode& mode)
{
	ni::Id<ni::GameObjectTag> id = mode.CreateGameObject();

	auto graphics = std::make_unique<ni::AnimatedGraphicsComponent>(ni::FileUtility::RemoveRelativePaths(tileset.texture_key_), tileset.tile_size_, 0);

	ni::Animation shine;
	shine.animation_row = 0;
	shine.frame_count = tileset.columns_;
	shine.start_frame = 1;
	shine.key_ = "shine";

	graphics->RegisterAnimation(shine);
	graphics->Play("shine", .15, true);

	ni::TransformComponent transform;
	transform.GetTransformable().setPosition(object.position_);
	transform.GetTransformable().setOrigin({tileset.tile_size_.x / 2.0f, tileset.tile_size_.y / 2.0f });

	mode.GetComponentStore().RegisterTagForId        (id, PlatformerGameMode::kSlingshotAmmoTag);
	mode.GetComponentStore().AttachGraphicsComponent (id, std::move(graphics));
	mode.GetComponentStore().AttachTransformComponent(id, transform);
}
