#include "AmmoUpdateComponent.h"

#include <box2d.h>
#include <math_functions.h>
#include <types.h>
#include <id.h>

#include <iostream>

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <NiEngine/DynamicBodyPhysicsComponent.h>
#include <NiEngine/Converter.h>
#include <NiEngine/TransformComponent.h>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/UpdateComponent.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/ServiceLocator.h>
#include <NiEngine/GameMode.h>
#include <NiEngine/CollisionCategories.h>
#include <NiEngine/Engine.h>

#include "CollisionCategories.h"
#include "PlatformerObjectFactory.h"

AmmoUpdateComponent::AmmoUpdateComponent(ni::Id<ni::GameObjectTag> owner_id, ni::ComponentLocator& component_locator, float max_speed) : UpdateComponent(component_locator)
{
	max_speed_ = max_speed;

	owner_id_ = owner_id;

	ni::ServiceLocator::Instance().GetEventDispatcher().OnMouseButtonPressed([this](const sf::Event::MouseButtonPressed& event) {
		if (active_ && !abilities_used_ && event.button == sf::Mouse::Button::Left)
		{ 
			ActivateAbilities();
		}
	});
}

void AmmoUpdateComponent::Launch(b2Vec2 direction, float impulse_ratio)
{
	direction *= -1;
	b2Vec2 velocity = direction * (impulse_ratio * max_speed_);

	Launch(velocity);
}

void AmmoUpdateComponent::Launch(b2Vec2 velocity)
{
	auto ammo_physics = static_cast<ni::DynamicBodyPhysicsComponent*>(component_locator_.GetPhysicsComponent(owner_id_));
	ni::TransformComponent* ammo_transform = component_locator_.GetTransformComponent(owner_id_);

	if (!ammo_physics || !ammo_transform)
	{
#ifdef _DEBUG
		std::cout << "\nAmmo physics and transform not found!!!";
#endif // _DEBUG
		return;
	}
	b2Body_SetTransform(ammo_physics->GetBodyId(), ni::Converter::PixelsToMeters(ammo_transform->GetTransformable().getPosition()), b2Rot_identity);

	ammo_physics->Activate();
	b2Body_SetLinearVelocity(ammo_physics->GetBodyId(), velocity);

	active_ = true;
	spawn_particles_ = true;

	time_since_launch_ = ni::Engine::time_elapsed;
}

void AmmoUpdateComponent::Update()
{
	if (tangible_)
	{
		return;
	}
	float elapsed_time = (ni::Engine::time_elapsed - time_since_launch_).asSeconds();
	if (elapsed_time < collision_cooldown_in_seconds)
	{
		return;
	}
	auto ammo_physics = static_cast<ni::DynamicBodyPhysicsComponent*>(component_locator_.GetPhysicsComponent(owner_id_));
	b2ShapeId shape_array[1]{};
	int shape_count = b2Body_GetShapes(ammo_physics->GetBodyId(), shape_array, 1);
	if (shape_count == 0)
	{
		return;
	}
	b2Filter new_filter = {};
	new_filter.categoryBits = CollisionCategories::kProjectiles;
	new_filter.maskBits     = ni::CollisionCategories::kTilemap | CollisionCategories::kProjectiles;

	b2Shape_SetFilter(shape_array[0], new_filter);
	tangible_ = true;
}

void AmmoUpdateComponent::SpawnComponents(ni::GameMode& mode)
{
	if (!particles_spawned_ && spawn_particles_)
	{
		PlatformerObjectFactory::SpawnAmmoParticles(mode, owner_id_);

		particles_spawned_ = true;
		spawn_particles_   = false;
	}
}

b2ShapeDef AmmoUpdateComponent::GetAmmoShapeDefinition()
{
	b2ShapeDef projectile_shape_def = b2DefaultShapeDef();
	projectile_shape_def.density = 10.0f;
	projectile_shape_def.material.restitution = 0.25f;
	projectile_shape_def.material.friction = 0.3f;
	projectile_shape_def.enableHitEvents = true;
	projectile_shape_def.filter.categoryBits = CollisionCategories::kProjectiles;
	projectile_shape_def.filter.maskBits     = ni::CollisionCategories::kTilemap;
	return projectile_shape_def;
}

void AmmoUpdateComponent::ActivateAbilities(bool child_ammo)
{
	abilities_used_ = true;

	for (auto& ability : ammo_abilities_)
	{
		if (child_ammo && !ability->propagate_)
		{
			continue;
		}		
		ability->Activate(component_locator_, owner_id_);
	}
}
