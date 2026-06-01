#include "PlatformerObjectFactory.h"

#include <types.h>
#include <box2d.h>
#include <id.h>
#include <collision.h>

#include <vector>
#include <memory>
#include <utility>

#include <SFML/System/Angle.hpp>
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
#include "MultiplicateAmmoAbility.h"
#include "TrailUpdateComponent.h"
#include "TrailGraphicsComponent.h"

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

ni::Id<ni::GameObjectTag> PlatformerObjectFactory::SpawnAmmo(ni::ObjectBlueprint object, ni::ObjectTemplateBlueprint& object_template, ni::TilesetBlueprint& tileset, ni::GameMode& mode)
{
	// Initializing some variables
	ni::TileBlueprint tile = tileset.tiles_.at(object_template.tile_gid_ - tileset.first_gid_ - 1);

	ni::Id<ni::GameObjectTag> id = mode.CreateGameObject();

	float max_speed = GetAttributeFromObject<float>(object, object_template, "max_speed");

	auto update = std::make_unique<AmmoUpdateComponent>(id, mode.GetComponentStore(), max_speed);
	
	b2ShapeDef projectile_shape_def = update->GetAmmoShapeDefinition();

	// Registering Ammo Abilities
	float speed_multiplier  = GetAttributeFromObject<float>(object, object_template, "speed_multiplier");
	float growth_multiplier = GetAttributeFromObject<float>(object, object_template, "growth_multiplier");

	float multiplication_angle = GetAttributeFromObject<float>(object, object_template, "multiplication_angle");
	float multiplication_count = GetAttributeFromObject<int  >(object, object_template, "multiplication_count");

	if (speed_multiplier > 0)
	{
		update->RegisterAbility(std::make_unique<ImpulseAmmoAbility>(speed_multiplier));
	}
	if (growth_multiplier > 0)
	{
		update->RegisterAbility(std::make_unique<GrowthAmmoAbility >(tile.polygon_blueprint_, tileset.tile_size_, growth_multiplier, projectile_shape_def, true));
	}	
	if (multiplication_angle > 0 && multiplication_count > 0)
	{
		update->RegisterAbility(std::make_unique<MultiplicateAmmoAbility>(object, object_template, tileset, &mode, tileset.tile_size_, multiplication_count, sf::degrees(multiplication_angle)));
	}

	// Defining the body and shape  
	b2BodyDef projectile_body_def     = b2DefaultBodyDef();
	projectile_body_def.type          = b2_dynamicBody;
	projectile_body_def.isBullet      = true;
	projectile_body_def.gravityScale  = 2.0f;
	projectile_body_def.linearDamping = 0.05f;
	projectile_body_def.enableSleep   = true;

	b2BodyId body_id = b2CreateBody(mode.GetPhysicsEngine().GetWorldId(), &projectile_body_def);

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

	return id;
}

ni::Id<ni::GameObjectTag> PlatformerObjectFactory::SpawnAmmoParticles(ni::GameMode& mode, ni::Id<ni::GameObjectTag> target_id)
{
	ni::Id<ni::GameObjectTag> id = mode.CreateGameObject();

	auto update   = std::make_unique<TrailUpdateComponent  >(mode.GetComponentStore(), id, target_id);
	auto graphics = std::make_unique<TrailGraphicsComponent>();

	ni::TransformComponent transform;

	mode.GetComponentStore().AttachUpdateComponent  (id, std::move(update));
	mode.GetComponentStore().AttachGraphicsComponent(id, std::move(graphics));
	mode.GetComponentStore().AttachTransformComponent(id, transform);

	return ni::Id<ni::GameObjectTag>();
}
