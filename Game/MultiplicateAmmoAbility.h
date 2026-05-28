#pragma once

#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/TilesetBlueprint.h>
#include <NiEngine/ObjectTemplateBlueprint.h>
#include <NiEngine/GameMode.h>
#include <NiEngine/ObjectBlueprint.h>
#include <NiEngine/DynamicBodyPhysicsComponent.h>
#include <NiEngine/TransformComponent.h>

#include "AmmoAbility.h"

class MultiplicateAmmoAbility : public AmmoAbility
{
public:
	MultiplicateAmmoAbility(
		ni::ObjectBlueprint object, 
		ni::ObjectTemplateBlueprint object_template, 
		ni::TilesetBlueprint tileset, 
		ni::GameMode* mode, 
		sf::Vector2i sprite_size,
		int amount,
		sf::Angle spread,
		bool propagate = false
	);

	void Activate(ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> owner_id) override;

private:
	ni::ObjectBlueprint object_blueprint_; 
	ni::ObjectTemplateBlueprint object_template_;
	ni::TilesetBlueprint tileset_;
	
	ni::GameMode* mode_ = nullptr;

	sf::Vector2i sprite_size_ = {};

	int amount_ = 0;
	sf::Angle spread_ = {};

	void SpawnAmmo(
		ni::ComponentLocator& component_locator, 
		ni::TransformComponent& owner_transform, 
		ni::DynamicBodyPhysicsComponent& owner_physics, 
		sf::Angle spread, 
		sf::Vector2f offset);
};

