#include "MultiplicateAmmoAbility.h"

#include <box2d.h>
#include <math_functions.h>

#include <iostream>

#include <SFML/System/Angle.hpp>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/GameMode.h>
#include <NiEngine/ObjectBlueprint.h>
#include <NiEngine/ObjectTemplateBlueprint.h>
#include <NiEngine/TilesetBlueprint.h>
#include <NiEngine/DynamicBodyPhysicsComponent.h>
#include <NiEngine/TransformComponent.h>
#include <NiEngine/Converter.h>

#include "PlatformerObjectFactory.h"
#include "AmmoUpdateComponent.h"
#include "AmmoAbility.h"
#include <SFML/System/Vector2.hpp>

MultiplicateAmmoAbility::MultiplicateAmmoAbility(
	ni::ObjectBlueprint object,
	ni::ObjectTemplateBlueprint object_template,
	ni::TilesetBlueprint tileset,
	ni::GameMode* mode,
	int amount,
	sf::Angle spread,
	bool propagate
)
	: AmmoAbility(propagate)
{
	object_blueprint_ = object;
	object_template_  = object_template;
	tileset_          = tileset;
	mode_             = mode;

	amount_ = amount;
	spread_ = spread;
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

	ni::TransformComponent* owner_transform = component_locator.GetTransformComponent(owner_id);
	auto owner_physics = static_cast<ni::DynamicBodyPhysicsComponent*>(component_locator.GetPhysicsComponent(owner_id));

	for (int i = 0; i < amount_; ++i)
	{
		sf::Angle ammo_spread = spread_ * i;

		SpawnAmmo(component_locator, *owner_transform, *owner_physics);
	}
}

void MultiplicateAmmoAbility::SpawnAmmo(ni::ComponentLocator& component_locator, ni::TransformComponent& owner_transform, ni::DynamicBodyPhysicsComponent& owner_physics, sf::Angle spread)
{	
	ni::Id<ni::GameObjectTag> ammo_id = PlatformerObjectFactory::SpawnAmmo(object_blueprint_, object_template_, tileset_, *mode_);

	ni::TransformComponent* ammo_transform = component_locator.GetTransformComponent(ammo_id);
	ammo_transform->GetTransformable().setPosition(owner_transform.GetTransformable().getPosition());

	auto ammo_update = static_cast<AmmoUpdateComponent*>(component_locator.GetUpdateComponent(ammo_id));

	sf::Vector2f velocity = ni::Converter::MetersToPixels(b2Body_GetLinearVelocity(owner_physics.GetBodyId()));
	velocity.rotatedBy(spread);

	ammo_update->Launch(ni::Converter::PixelsToMeters(velocity));
	ammo_update->ActivateAbilities(true);
}
