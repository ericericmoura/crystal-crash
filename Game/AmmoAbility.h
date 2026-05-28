#pragma once

#include <NiEngine/Id.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/ComponentLocator.h>

class AmmoAbility
{
public:
	bool propagate_ = false;

	AmmoAbility(bool propagate = false) : propagate_(propagate) {};

	virtual void Activate(ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> ammo_id) = 0;
};

