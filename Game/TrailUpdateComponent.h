#pragma once

#include <SFML/System/Time.hpp>
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
	ni::Id<ni::GameObjectTag> target_id_;

	float delay_between_particle_spawn_in_seconds_ = 1.0f;
	sf::Time time_since_last_particle_spawn_ = {};
};

