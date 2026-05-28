#pragma once

#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/ComponentLocator.h>

#include "AmmoAbility.h"

class ImpulseAmmoAbility : public AmmoAbility
{
public:	
	ImpulseAmmoAbility(float impulse_multiplier, bool propagate = false);
	void Activate(ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> ammo_id) override;

private:
	float impulse_multiplier_ = 0;
};

