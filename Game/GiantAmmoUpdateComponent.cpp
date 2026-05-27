#include "GiantAmmoUpdateComponent.h"

#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>

#include "AmmoUpdateComponent.h"

GiantAmmoUpdateComponent::GiantAmmoUpdateComponent(ni::Id<ni::GameObjectTag> owner_id, ni::ComponentLocator& component_locator, float grow_factor) : 
	AmmoUpdateComponent(owner_id, component_locator)
{}

void GiantAmmoUpdateComponent::ActivateAbility()
{

}
