#pragma once

#include <NiEngine/Id.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/ComponentLocator.h>

class AmmoAbility
{
public:
	virtual void Activate(ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> ammo_id) = 0;
};

