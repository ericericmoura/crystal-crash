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
	sf::Vector2i sprite_size,
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

	sprite_size_ = sprite_size;
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

	int half_amount = amount_ / 2.0f;

	sf::Vector2f velocity = ni::Converter::MetersToPixels(b2Body_GetLinearVelocity(owner_physics->GetBodyId()));
	if (velocity.length() == 0)
	{
		return;
	}
	sf::Vector2f perp = sf::Vector2f(-velocity.y, velocity.x).normalized();

	sf::Vector2f offset_spacing(sprite_size_);
	offset_spacing.x *= component_locator.GetTransformComponent(owner_id)->GetTransformable().getScale().x + 1;
	offset_spacing.y *= component_locator.GetTransformComponent(owner_id)->GetTransformable().getScale().y + 1;

	for (int i = 0; i < half_amount; ++i)
	{
		sf::Angle ammo_spread = spread_ * (i + 1);
		
		sf::Vector2f offset;
		offset.x = perp.x * offset_spacing.x * (i + 1);
		offset.y = perp.y * offset_spacing.y * (i + 1);

		SpawnAmmo(component_locator, *owner_transform, *owner_physics, ammo_spread, offset);
	}
	for (int i = 0; i < half_amount; ++i)
	{
		sf::Angle ammo_spread = spread_ * -(i + 1);

		sf::Vector2f offset;
		offset.x = -perp.x * offset_spacing.x * (i + 1);
		offset.y = -perp.y * offset_spacing.y * (i + 1);

		SpawnAmmo(component_locator, *owner_transform, *owner_physics, ammo_spread, offset);
	}
}

void MultiplicateAmmoAbility::SpawnAmmo(ni::ComponentLocator& component_locator, ni::TransformComponent& owner_transform, ni::DynamicBodyPhysicsComponent& owner_physics, sf::Angle spread, sf::Vector2f offset)
{		
	sf::Vector2f owner_velocity = ni::Converter::MetersToPixels(b2Body_GetLinearVelocity(owner_physics.GetBodyId()));

	if (owner_velocity.length() == 0)
	{
		return;
	}

	sf::Vector2f base_direction = owner_velocity.normalized().rotatedBy(spread);
	sf::Vector2f velocity       = base_direction * owner_velocity.length();

	ni::Id<ni::GameObjectTag> ammo_id = PlatformerObjectFactory::SpawnAmmo(object_blueprint_, object_template_, tileset_, *mode_);

	ni::TransformComponent* ammo_transform = component_locator.GetTransformComponent(ammo_id);
	ammo_transform->GetTransformable().setPosition(owner_transform.GetTransformable().getPosition() + offset);

	auto ammo_update = static_cast<AmmoUpdateComponent*>(component_locator.GetUpdateComponent(ammo_id));

	ammo_update->Launch(ni::Converter::PixelsToMeters(velocity));
	ammo_update->ActivateAbilities(true);
}
