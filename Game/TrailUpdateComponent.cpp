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
	float time_elapsed_in_seconds = (time_since_last_particle_spawn_ - ni::Engine::time_elapsed).asSeconds();
	if (time_elapsed_in_seconds >= delay_between_particle_spawn_in_seconds_)
	{
		ni::TransformComponent* target_transform = component_locator_.GetTransformComponent(target_id_);

		auto graphics  = static_cast<TrailGraphicsComponent*>(component_locator_.GetGraphicsComponents(owner_id_).front());
		sf::Vector2f particle_position = target_transform->GetTransformable().getPosition();
		
		graphics->SpawnParticle(particle_position);

		time_since_last_particle_spawn_ = ni::Engine::time_elapsed;
	}
}
