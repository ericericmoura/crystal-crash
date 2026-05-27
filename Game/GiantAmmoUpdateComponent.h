#pragma once
#include "AmmoUpdateComponent.h"

#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>

class GiantAmmoUpdateComponent : public AmmoUpdateComponent
{
public:
	GiantAmmoUpdateComponent(ni::Id<ni::GameObjectTag> owner_id, ni::ComponentLocator& component_locator, float grow_factor);

private:
	float grow_factor_ = 2;

	virtual void ActivateAbility();
};

