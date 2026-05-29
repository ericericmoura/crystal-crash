#include "TrailUpdateComponent.h"

#include <SFML/System/Vector2.hpp>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/UpdateComponent.h>
#include <NiEngine/Engine.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/TransformComponent.h>

#include "TrailGraphicsComponent.h"

TrailUpdateComponent::TrailUpdateComponent(ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> owner_id, ni::Id<ni::GameObjectTag> target_id) :
	UpdateComponent(component_locator),
	target_id_(target_id)
{
	owner_id_ = owner_id;
}

void TrailUpdateComponent::Update()
{
	ni::TransformComponent* target_transform = component_locator_.GetTransformComponent(target_id_);
	
	float distance = (last_particle_position_ - target_transform->GetTransformable().getPosition()).length();
	
	if (distance > kDistanceBetweenParticleSpawn)
	{
		last_particle_position_ = target_transform->GetTransformable().getPosition();
		auto graphics  = static_cast<TrailGraphicsComponent*>(component_locator_.GetGraphicsComponents(owner_id_).front());
		
		graphics->SpawnParticle(last_particle_position_);
	}
}
