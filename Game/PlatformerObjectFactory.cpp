#include "PlatformerObjectFactory.h"

#include <types.h>
#include <box2d.h>
#include <id.h>
#include <collision.h>

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
#include <NiEngine/DynamicBodyPhysicsComponent.h>
#include <NiEngine/Converter.h>

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

	b2BodyDef projectile_body_def     = b2DefaultBodyDef();
	projectile_body_def.type          = b2_dynamicBody;
	projectile_body_def.isBullet      = true;
	projectile_body_def.gravityScale  = 2.0f;
	projectile_body_def.linearDamping = 0.05f;
	projectile_body_def.enableSleep   = true;

	b2ShapeDef projectile_shape_def           = b2DefaultShapeDef();
	projectile_shape_def.density              = 10.0f;
	projectile_shape_def.material.restitution = 0.25f;
	projectile_shape_def.material.friction    = 0.3f;
	projectile_shape_def.enableHitEvents      = true;

	b2BodyId body_id = b2CreateBody(world_id_, &projectile_body_def);

	float circle_radius = ni::Converter::PixelsToMeters(20.0f);

	b2Circle circle{};
	circle.radius =   circle_radius;
	circle.center = { circle_radius/2.0f, circle_radius/2.0f };

	b2CreateCircleShape(body_id, &projectile_shape_def, &circle);

	auto update   = std::make_unique<ni::DynamicBodyPhysicsComponent>(body_id, false);

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
