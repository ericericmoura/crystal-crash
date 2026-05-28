#include "MultiplicateAmmoAbility.h"

#include <box2d.h>

#include <iostream>

#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/GameMode.h>
#include <NiEngine/ObjectBlueprint.h>
#include <NiEngine/ObjectTemplateBlueprint.h>
#include <NiEngine/TilesetBlueprint.h>
#include <NiEngine/DynamicBodyPhysicsComponent.h>
#include <NiEngine/TransformComponent.h>

#include "PlatformerObjectFactory.h"
#include "AmmoUpdateComponent.h"
#include "AmmoAbility.h"

MultiplicateAmmoAbility::MultiplicateAmmoAbility(ni::ObjectBlueprint object, ni::ObjectTemplateBlueprint object_template, ni::TilesetBlueprint tileset, ni::GameMode* mode, bool propagate)
	: AmmoAbility(propagate)
{
	object_blueprint_ = object;
	object_template_  = object_template;
	tileset_          = tileset;
	mode_             = mode;
}

void MultiplicateAmmoAbility::Activate(ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> owner_id)
{	
	if (!mode_)
	{
#ifdef _DEBUG
		std::cout << "\nMultiplicateAmmoAbility: Invalid Game Mode!";
#endif // _DEBUG
		return;
	}
	ni::Id<ni::GameObjectTag> ammo_id = PlatformerObjectFactory::SpawnAmmo(object_blueprint_, object_template_, tileset_, *mode_);

	ni::TransformComponent* owner_transform = component_locator.GetTransformComponent(owner_id);
	ni::TransformComponent* ammo_transform  = component_locator.GetTransformComponent(ammo_id );
	ammo_transform->GetTransformable().setPosition(owner_transform->GetTransformable().getPosition());

	auto owner_physics = static_cast<ni::DynamicBodyPhysicsComponent*>(component_locator.GetPhysicsComponent(owner_id));
	auto ammo_update   = static_cast<AmmoUpdateComponent*>(component_locator.GetUpdateComponent(ammo_id ));
	
	ammo_update->Launch(b2Body_GetLinearVelocity(owner_physics->GetBodyId()));
	ammo_update->ActivateAbilities(true);
}

void MultiplicateAmmoAbility::SpawnAmmo(ni::ComponentLocator& component_locator)
{	
}
