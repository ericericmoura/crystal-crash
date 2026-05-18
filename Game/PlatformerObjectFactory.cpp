#include "PlatformerObjectFactory.h"

#include <vector>
#include <string>
#include <utility>
#include <memory>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <NiEngine/TilesetBlueprint.h>
#include <NiEngine/GameMode.h>
#include <NiEngine/ObjectTemplateBlueprint.h>
#include <NiEngine/ObjectBlueprint.h>
#include <NiEngine/TiledUtility.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/StandardGraphicsComponent.h>
#include <NiEngine/TransformComponent.h>
#include <NiEngine/AnimatedGraphicsComponent.h>

#include "ObstacleHarmfulCollisionComponent.h"
#include "MovingObstacleUpdateComponent.h"
#include "PlatformerGameMode.h"
#include "CharacterPhysicsComponent.h"
#include "PlayerUpdateComponent.h"
#include "ExitDoorUpdateComponent.h"
#include "ObstacleSolidCollisionComponent.h"
#include "WalkerEnemyUpdateComponent.h"
#include "JumperEnemyUpdateComponent.h"

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
	case ObjectTypes::Spike:
		SpawnMovingObject(object, object_template, texture_key, texture_coords, mode, PlatformerGameMode::kSpikeTag);
		break;
	case ObjectTypes::Ground:
		SpawnMovingObject(object, object_template, texture_key, texture_coords, mode, "");
		break;
	case ObjectTypes::Player:
		SpawnPlayer(object, object_template, texture_key, texture_coords, mode);
		break;
	case ObjectTypes::ExitDoor:
		SpawnExitDoor(object, object_template, texture_key, texture_coords, mode);
		break;
	case ObjectTypes::WalkerEnemy:
		SpawnWalkerEnemy(object, object_template, texture_key, texture_coords, mode);
		break;
	case ObjectTypes::JumperEnemy:
		SpawnJumperEnemy(object, object_template, texture_key, texture_coords, mode);
		break;
	}
}

// SPAWN PLAYER
void PlatformerObjectFactory::SpawnPlayer(ni::ObjectBlueprint object, ni::ObjectTemplateBlueprint& object_template, std::string texture_key, sf::IntRect texture_coordinates, ni::GameMode& mode)
{
	ni::Id<ni::GameObjectTag> id = mode.CreateGameObject();

	object.position_.x += texture_coordinates.size.x / 2.0f;
	object.position_.y += texture_coordinates.size.y / 2.0f;

	auto physics  = std::make_unique<CharacterPhysicsComponent>(texture_coordinates.size);

	physics->SetGravity  (GetAttributeFromObject<float>(object, object_template, "gravity"   ));
	physics->SetSpeed    (GetAttributeFromObject<float>(object, object_template, "speed"     ));
	physics->SetJumpForce(GetAttributeFromObject<float>(object, object_template, "jump_force"));

	auto graphics = std::make_unique<ni::AnimatedGraphicsComponent>(texture_key, texture_coordinates.size, 1);

	auto& platformer_mode = static_cast<PlatformerGameMode&>(mode);
	auto update   = std::make_unique<PlayerUpdateComponent>(mode.GetComponentStore(), id, platformer_mode, sf::Vector2f(texture_coordinates.size));

	ni::TransformComponent transform;
	transform.GetTransformable().setPosition(object.position_);
	transform.GetTransformable().setOrigin({ texture_coordinates.size.x / 2.0f, texture_coordinates.size.y / 2.0f });

	update->Init(*graphics.get(), *physics.get());

	mode.GetComponentStore().RegisterTagForId        (id, PlatformerGameMode::kPlayerTag);
	mode.GetComponentStore().AttachPhysicsComponent  (id, std::move(physics));
	mode.GetComponentStore().AttachUpdateComponent   (id, std::move(update));	
	mode.GetComponentStore().AttachGraphicsComponent (id, std::move(graphics));
	mode.GetComponentStore().AttachTransformComponent(id, transform);
}

// SPAWN MOVING OBJECT
void PlatformerObjectFactory::SpawnMovingObject(ni::ObjectBlueprint object, ni::ObjectTemplateBlueprint& object_template, std::string texture_key, sf::IntRect texture_coordinates, ni::GameMode& mode, std::string object_tag)
{
	sf::Vector2f movement_offset = {};
	movement_offset.y = GetAttributeFromObject<float>(object, object_template, "movement_offset_y");
	movement_offset.x = GetAttributeFromObject<float>(object, object_template, "movement_offset_x");

	float movement_delay = GetAttributeFromObject<float>(object, object_template, "movement_delay");

	sf::Vector2i repeat;
	repeat.y = GetAttributeFromObject<int>(object, object_template, "repeat_y");
	repeat.x = GetAttributeFromObject<int>(object, object_template, "repeat_x");

	bool harmful = GetAttributeFromObject<bool>(object, object_template, "harmful");

	float trigger_distance_y = GetAttributeFromObject<float>(object, object_template, "trigger_distance_y");
	float trigger_distance_x = GetAttributeFromObject<float>(object, object_template, "trigger_distance_x");	

	ni::Id<ni::GameObjectTag> id = mode.CreateGameObject();

	if (object_tag != "")
	{
		mode.GetComponentStore().RegisterTagForId(id, object_tag);
	}

	auto graphics = std::make_unique<ni::StandardGraphicsComponent>(texture_key, texture_coordinates);
	graphics->SetRepeating(repeat);

	sf::Vector2f object_top_left_position = object.position_;
	object_top_left_position.y -= texture_coordinates.size.y;	

	ni::TransformComponent transform;
	transform.GetTransformable().setPosition(object_top_left_position);

	sf::Vector2f collision_size;
	collision_size.x = texture_coordinates.size.x + texture_coordinates.size.x * repeat.x;
	collision_size.y = texture_coordinates.size.y + texture_coordinates.size.y * repeat.y;	

	if (trigger_distance_y == 0)
	{
		trigger_distance_y = collision_size.y / 2.0f;
	}
	if (trigger_distance_x == 0)
	{
		trigger_distance_x = collision_size.x / 2.0f;
	}

	auto update = std::make_unique<MovingObstacleUpdateComponent>(
		mode.GetComponentStore(), transform, id,
		sf::Vector2i(movement_offset), sf::Vector2f({ trigger_distance_x, trigger_distance_y }),
		collision_size,
		movement_delay		
	);

	if (harmful)
	{
		update->RegisterCollisionComponent(std::make_unique<ObstacleHarmfulCollisionComponent>());
	}
	else
	{
		update->RegisterCollisionComponent(std::make_unique<ObstacleSolidCollisionComponent>());
	}

	mode.GetComponentStore().AttachUpdateComponent   (id, std::move(update));
	mode.GetComponentStore().AttachGraphicsComponent (id, std::move(graphics));
	mode.GetComponentStore().AttachTransformComponent(id, transform);
}

// SPAWN EXIT
void PlatformerObjectFactory::SpawnExitDoor(ni::ObjectBlueprint object, ni::ObjectTemplateBlueprint& object_template, std::string texture_key, sf::IntRect texture_coordinates, ni::GameMode& mode)
{
	ni::Id<ni::GameObjectTag> id = mode.CreateGameObject();

	auto graphics = std::make_unique<ni::AnimatedGraphicsComponent>(texture_key, texture_coordinates.size, 1);
	graphics->SetFrame(2, 16);

	auto update = std::make_unique<ExitDoorUpdateComponent>(mode, mode.GetComponentStore(), id, sf::Vector2f(texture_coordinates.size));

	sf::Vector2f object_top_left_position = object.position_;
	object_top_left_position.y -= texture_coordinates.size.y;

	ni::TransformComponent transform;
	transform.GetTransformable().setPosition(object_top_left_position);

	mode.GetComponentStore().AttachUpdateComponent   (id, std::move(update));
	mode.GetComponentStore().AttachGraphicsComponent (id, std::move(graphics));
	mode.GetComponentStore().AttachTransformComponent(id, transform);
}

void PlatformerObjectFactory::SpawnWalkerEnemy(ni::ObjectBlueprint object, ni::ObjectTemplateBlueprint& object_template, std::string texture_key, sf::IntRect texture_coordinates, ni::GameMode& mode)
{
	ni::Id<ni::GameObjectTag> id = mode.CreateGameObject();

	object.position_.x += texture_coordinates.size.x / 2.0f;
	object.position_.y += texture_coordinates.size.y / 2.0f;

	auto physics  = std::make_unique<CharacterPhysicsComponent>(texture_coordinates.size);
	physics->SetSpeed(GetAttributeFromObject<float>(object, object_template, "speed"));

	auto graphics = std::make_unique<ni::AnimatedGraphicsComponent>(texture_key, texture_coordinates.size, 1);

	auto update = std::make_unique<WalkerEnemyUpdateComponent>(mode.GetComponentStore(), id, GetAttributeFromObject<int>(object, object_template, "initial_walking_direction"), sf::Vector2f(texture_coordinates.size));

	ni::TransformComponent transform;
	transform.GetTransformable().setPosition(object.position_);
	transform.GetTransformable().setOrigin({ texture_coordinates.size.x / 2.0f, texture_coordinates.size.y / 2.0f });

	update->Init(*graphics.get(), *physics.get());

	mode.GetComponentStore().RegisterTagForId        (id, PlatformerGameMode::kEnemyTag);
	mode.GetComponentStore().AttachPhysicsComponent  (id, std::move(physics));
	mode.GetComponentStore().AttachUpdateComponent   (id, std::move(update));
	mode.GetComponentStore().AttachGraphicsComponent (id, std::move(graphics));
	mode.GetComponentStore().AttachTransformComponent(id, transform);
}

void PlatformerObjectFactory::SpawnJumperEnemy(ni::ObjectBlueprint object, ni::ObjectTemplateBlueprint& object_template, std::string texture_key, sf::IntRect texture_coordinates, ni::GameMode& mode)
{
	ni::Id<ni::GameObjectTag> id = mode.CreateGameObject();

	object.position_.x += texture_coordinates.size.x / 2.0f;
	object.position_.y += texture_coordinates.size.y / 2.0f;

	auto physics = std::make_unique<CharacterPhysicsComponent>(texture_coordinates.size);
	physics->SetSpeed    (GetAttributeFromObject<float>(object, object_template, "speed"));
	physics->SetJumpForce(GetAttributeFromObject<float>(object, object_template, "jump_force"));
	physics->SetGravity  (GetAttributeFromObject<float>(object, object_template, "gravity"));

	auto graphics = std::make_unique<ni::AnimatedGraphicsComponent>(texture_key, texture_coordinates.size, 1);

	auto update = std::make_unique<JumperEnemyUpdateComponent>(mode.GetComponentStore(), id, sf::Vector2f(texture_coordinates.size), GetAttributeFromObject<float>(object, object_template, "jump_cooldown"), GetAttributeFromObject<float>(object, object_template, "upside_down_cooldown"));

	ni::TransformComponent transform;
	transform.GetTransformable().setPosition(object.position_);
	transform.GetTransformable().setOrigin({ texture_coordinates.size.x / 2.0f, texture_coordinates.size.y / 2.0f });

	update->Init(*graphics.get(), *physics.get());

	mode.GetComponentStore().RegisterTagForId(id, PlatformerGameMode::kEnemyTag);
	mode.GetComponentStore().AttachPhysicsComponent(id, std::move(physics));
	mode.GetComponentStore().AttachUpdateComponent(id, std::move(update));
	mode.GetComponentStore().AttachGraphicsComponent(id, std::move(graphics));
	mode.GetComponentStore().AttachTransformComponent(id, transform);
}
