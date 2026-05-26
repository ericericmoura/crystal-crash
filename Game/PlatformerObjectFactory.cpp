#include "PlatformerObjectFactory.h"

#include <types.h>
#include <box2d.h>
#include <id.h>
#include <collision.h>
#include <math_functions.h>

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
#include <NiEngine/Converter.h>
#include <NiEngine/StandardGraphicsComponent.h>

#include "PlatformerGameMode.h"
#include "SlingshotUpdateComponent.h"
#include "AmmoUpdateComponent.h"

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

	ni::TileBlueprint tile = tileset.tiles_.at(object_template.tile_gid_ - tileset.first_gid_ - 1);
	tile.polygon_blueprint_.offset_points_;

	const int kVerticesCount = tile.polygon_blueprint_.offset_points_.size();
	
	sf::Vector2i initial_vertice = tile.polygon_blueprint_.position_;

	sf::Vector2i half_size = { tileset.tile_size_.x / 2, tileset.tile_size_.y / 2 };

	std::vector<b2Vec2> points = {};
	for (int i = 0; i < kVerticesCount; ++i)
	{
		auto offset = tile.polygon_blueprint_.offset_points_.at(i);
		auto vertice_position = initial_vertice + offset;

		vertice_position -= half_size;

		points.push_back(ni::Converter::PixelsToMeters(vertice_position));
	}

	b2Hull hull       = b2ComputeHull(points.data(), kVerticesCount);
	b2Polygon polygon = b2MakePolygon(&hull, 0);	

	b2CreatePolygonShape(body_id, &projectile_shape_def, &polygon);

	auto update = std::make_unique<AmmoUpdateComponent>(id, mode.GetComponentStore());

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
