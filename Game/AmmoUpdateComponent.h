#pragma once

#include <vector>
#include <memory>
#include <utility>

#include <math_functions.h>
#include <types.h>

#include <NiEngine/UpdateComponent.h>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/GameMode.h>

#include "AmmoAbility.h"

class AmmoUpdateComponent : public ni::UpdateComponent
{
public:
	AmmoUpdateComponent(ni::Id<ni::GameObjectTag> owner_id, ni::ComponentLocator& component_locator, float max_speed);

	void Launch(b2Vec2 direction, float impulse_ratio);
	void Launch(b2Vec2 velocity);

	void Update() override;
	void SpawnComponents(ni::GameMode& mode) override;

	void RegisterAbility(std::unique_ptr<AmmoAbility> ability)
	{
		ammo_abilities_.emplace_back(std::move(ability));
	}

	b2ShapeDef GetAmmoShapeDefinition();

	void ActivateAbilities(bool child_ammo = false);

private:
	float max_speed_ = 40.0f;

	bool abilities_used_ = false;
	bool active_         = false;

	bool spawn_particles_   = false;
	bool particles_spawned_ = false;

	std::vector<std::unique_ptr<AmmoAbility>> ammo_abilities_ = {};
};

