#include "ImpulseAmmoAbility.h"

#include <box2d.h>
#include <math_functions.h>

#include <NiEngine/GameObjectTag.h>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/DynamicBodyPhysicsComponent.h>
#include <NiEngine/Id.h>
#include "AmmoAbility.h"

ImpulseAmmoAbility::ImpulseAmmoAbility(float impulse_multiplier, bool propagate) : AmmoAbility(propagate), impulse_multiplier_(impulse_multiplier)
{}

void ImpulseAmmoAbility::Activate(ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> ammo_id)
{
	auto ammo_physics = static_cast<ni::DynamicBodyPhysicsComponent*>(component_locator.GetPhysicsComponent(ammo_id));

	b2Vec2 velocity = b2Body_GetLinearVelocity(ammo_physics->GetBodyId());
	velocity *= impulse_multiplier_;
	b2Body_SetLinearVelocity(ammo_physics->GetBodyId(), velocity);
}
