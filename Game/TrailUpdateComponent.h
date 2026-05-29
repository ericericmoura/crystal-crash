#pragma once

#include <SFML/System/Vector2.hpp>
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
	inline static const float kDistanceBetweenParticleSpawn = 25.0f;

	ni::Id<ni::GameObjectTag> target_id_;

	sf::Vector2f last_particle_position_;

	sf::Time time_since_stopped_    = {};
	float despawn_delay_in_seconds_ = 5.0f;
	bool stop_ = false;
};

