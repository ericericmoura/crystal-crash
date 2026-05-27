#pragma once

#include "AmmoUpdateComponent.h"

#include <SFML/System/Vector2.hpp>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/PolygonBlueprint.h>

class GiantAmmoUpdateComponent : public AmmoUpdateComponent
{
public:
	GiantAmmoUpdateComponent(ni::Id<ni::GameObjectTag> owner_id, ni::ComponentLocator& component_locator, float grow_factor, ni::PolygonBlueprint polygon_blueprint, sf::Vector2i tile_size);

private:
	float grow_factor_ = 2;

	ni::PolygonBlueprint polygon_blueprint_;
	sf::Vector2i tile_size_;

	virtual void ActivateAbility();
};

