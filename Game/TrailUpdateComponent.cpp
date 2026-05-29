#include "TrailUpdateComponent.h"

#include <box2d.h>
#include <math_functions.h>

#include <SFML/System/Vector2.hpp>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/UpdateComponent.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/TransformComponent.h>
#include <NiEngine/DynamicBodyPhysicsComponent.h>
#include <NiEngine/Engine.h>

#include "TrailGraphicsComponent.h"

TrailUpdateComponent::TrailUpdateComponent(ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> owner_id, ni::Id<ni::GameObjectTag> target_id) :
	UpdateComponent(component_locator),
	target_id_(target_id)
{
	owner_id_ = owner_id;
}

void TrailUpdateComponent::Update()
{
	if (stop_)
	{
		float time_elapsed = (ni::Engine::time_elapsed - time_since_stopped_).asSeconds();
		float fade_percentage = 1 - time_elapsed/despawn_delay_in_seconds_;

		auto graphics = static_cast<TrailGraphicsComponent*>(component_locator_.GetGraphicsComponents(owner_id_).front());
		graphics->SetAlpha(fade_percentage);

		return;
	}

	auto   target_physics  = static_cast<ni::DynamicBodyPhysicsComponent*>(component_locator_.GetPhysicsComponent(target_id_));
	b2Vec2 target_velocity = b2Body_GetLinearVelocity(target_physics->GetBodyId());
	if (target_velocity.x == 0 && target_velocity.y == 0)
	{
		stop_ = true;
		time_since_stopped_ = ni::Engine::time_elapsed;
		return;
	}

	ni::TransformComponent* target_transform = component_locator_.GetTransformComponent(target_id_);
	
	float distance = (last_particle_position_ - target_transform->GetTransformable().getPosition()).length();
	
	if (distance > kDistanceBetweenParticleSpawn)
	{
		last_particle_position_ = target_transform->GetTransformable().getPosition();
		auto graphics  = static_cast<TrailGraphicsComponent*>(component_locator_.GetGraphicsComponents(owner_id_).front());
		
		graphics->SpawnParticle(last_particle_position_);
	}
}
