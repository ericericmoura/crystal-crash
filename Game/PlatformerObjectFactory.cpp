#include "PlatformerObjectFactory.h"

#include <types.h>
#include <box2d.h>
#include <id.h>
#include <collision.h>

#include <vector>
#include <memory>
#include <utility>

#include <SFML/System/Vector2.hpp>
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
#include <NiEngine/DynamicBodyPhysicsComponent.h>
#include <NiEngine/StandardGraphicsComponent.h>
#include <NiEngine/PolygonUtility.h>

#include "PlatformerGameMode.h"
#include "SlingshotUpdateComponent.h"
#include "AmmoUpdateComponent.h"
#include "ImpulseAmmoAbility.h"
#include "GrowthAmmoAbility.h"

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
	// Creating chain
	ni::Id<ni::GameObjectTag> chain_id = mode.CreateGameObject();

	auto chain_graphics = std::make_unique<ni::StandardGraphicsComponent>("graphics/Chain - Silver.png");

	ni::TransformComponent chain_transform;
	chain_transform.GetTransformable().setOrigin({ 7.0f/2.0f, 0 });

	mode.GetComponentStore().AttachGraphicsComponent (chain_id, std::move(chain_graphics));
	mode.GetComponentStore().AttachTransformComponent(chain_id, chain_transform);

	// Creating Slingshot
	int gid = object_template.tile_gid_ - tileset.first_gid_ - 1;

	ni::TileBlueprint& tile = tileset.tiles_.at(gid);

	ni::Id<ni::GameObjectTag> id = mode.CreateGameObject();

	object.position_.y -= tile.image_size_.y;

	auto update   = std::make_unique<SlingshotUpdateComponent>(mode.GetComponentStore(), object.position_, tile.image_size_, chain_id);
	auto graphics = std::make_unique<ni::AnimatedGraphicsComponent>(ni::FileUtility::RemoveRelativePaths(tile.image_key_), tile.image_size_, 1);

	ni::TransformComponent transform;
	transform.GetTransformable().setPosition(object.position_);	

	mode.GetComponentStore().AttachUpdateComponent   (id, std::move(update));
	mode.GetComponentStore().AttachGraphicsComponent (id, std::move(graphics));
	mode.GetComponentStore().AttachTransformComponent(id, transform);
}

void PlatformerObjectFactory::SpawnAmmo(ni::ObjectBlueprint object, ni::ObjectTemplateBlueprint& object_template, ni::TilesetBlueprint& tileset, ni::GameMode& mode)
{
	ni::TileBlueprint tile = tileset.tiles_.at(object_template.tile_gid_ - tileset.first_gid_ - 1);

	ni::Id<ni::GameObjectTag> id = mode.CreateGameObject();

	auto update = std::make_unique<AmmoUpdateComponent>(id, mode.GetComponentStore());
	
	b2ShapeDef projectile_shape_def = update->GetAmmoShapeDefinition();

	// Registering Ammo Abilities
	update->RegisterAbility(std::make_unique<ImpulseAmmoAbility>(1.0f));
	update->RegisterAbility(std::make_unique<GrowthAmmoAbility >(tile.polygon_blueprint_, tileset.tile_size_, 2.0f, projectile_shape_def));

	// Defining the body and shape  
	b2BodyDef projectile_body_def     = b2DefaultBodyDef();
	projectile_body_def.type          = b2_dynamicBody;
	projectile_body_def.isBullet      = true;
	projectile_body_def.gravityScale  = 2.0f;
	projectile_body_def.linearDamping = 0.05f;
	projectile_body_def.enableSleep   = true;

	b2BodyId body_id = b2CreateBody(world_id_, &projectile_body_def);	

	b2Polygon polygon = ni::PolygonUtility::CreatePolygonFromOffsetPoints(tile.polygon_blueprint_, tileset.tile_size_);

	b2CreatePolygonShape(body_id, &projectile_shape_def, &polygon);

	// Creating components	
	auto physics  = std::make_unique<ni::DynamicBodyPhysicsComponent>(body_id, false);
	auto graphics = std::make_unique<ni::AnimatedGraphicsComponent>(ni::FileUtility::RemoveRelativePaths(tileset.texture_key_), tileset.tile_size_, 0);	

	ni::Animation shine;
	shine.animation_row = 0;
	shine.frame_count = tileset.columns_;
	shine.start_frame = 1;
	shine.key_ = "shine";

	graphics->RegisterAnimation(shine);
	graphics->Play("shine", 0.15f, true);

	ni::TransformComponent transform;
	transform.GetTransformable().setPosition(object.position_);
	transform.GetTransformable().setOrigin({tileset.tile_size_.x / 2.0f, tileset.tile_size_.y / 2.0f });

	mode.GetComponentStore().RegisterTagForId        (id, PlatformerGameMode::kSlingshotAmmoTag);
	mode.GetComponentStore().AttachUpdateComponent   (id, std::move(update  ));
	mode.GetComponentStore().AttachPhysicsComponent  (id, std::move(physics ));
	mode.GetComponentStore().AttachGraphicsComponent (id, std::move(graphics));
	mode.GetComponentStore().AttachTransformComponent(id, transform);
}
