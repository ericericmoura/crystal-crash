#pragma once

#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/ComponentLocator.h>

#include "AmmoAbility.h"

class ImpulseAmmoAbility : public AmmoAbility
{
public:	
	void Activate(ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> ammo_id) override;
};

