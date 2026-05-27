#pragma once

#include <vector>
#include <memory>

#include <math_functions.h>
#include <types.h>

#include <NiEngine/UpdateComponent.h>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>

#include <utility>
#include "AmmoAbility.h"

class AmmoUpdateComponent : public ni::UpdateComponent
{
public:
	AmmoUpdateComponent(ni::Id<ni::GameObjectTag> owner_id, ni::ComponentLocator& component_locator);

	void Launch(b2Vec2 direction, float impulse_ratio);
	void Update() override;

	void RegisterAbility(std::unique_ptr<AmmoAbility> ability)
	{
		ammo_abilities_.emplace_back(std::move(ability));
	}

	b2ShapeDef GetAmmoShapeDefinition();

private:
	float max_speed_ = 40.0f;

	bool ability_used_ = false;
	bool active_       = false;

	std::vector<std::unique_ptr<AmmoAbility>> ammo_abilities_ = {};

	void ActivateAbilities();
};

