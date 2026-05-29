#pragma once

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <NiEngine/UpdateComponent.h>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>

class TrailUpdateComponent : public ni::UpdateComponent
{
public:
	TrailUpdateComponent(ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> owner_id, ni::Id<ni::GameObjectTag> target_id);
	void Update() override;

private:
	inline static const float kDistanceBetweenParticleSpawn = 50.0f;

	ni::Id<ni::GameObjectTag> target_id_;

	//float delay_between_particle_spawn_in_seconds_ = 0.2f;
	//sf::Time time_since_last_particle_spawn_ = {};

	sf::Vector2f last_particle_position_;
};

