#pragma once

#include <types.h>

#include <SFML/System/Vector2.hpp>
#include <NiEngine/PolygonBlueprint.h>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>

#include "AmmoAbility.h"

class GrowthAmmoAbility : public AmmoAbility
{
public:
	GrowthAmmoAbility(ni::PolygonBlueprint polygon_blueprint, sf::Vector2i tile_size, float growth_multiplier, b2ShapeDef shape_definition, bool propagate = true);

	void Activate(ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> ammo_id) override;

private:

	b2ShapeDef shape_definition_ = {};

	float growth_multiplier_ = 0;

	ni::PolygonBlueprint polygon_blueprint_ = {};
	sf::Vector2i tile_size_ = {};
};

