#pragma once

#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/TilesetBlueprint.h>
#include <NiEngine/ObjectTemplateBlueprint.h>
#include <NiEngine/GameMode.h>
#include <NiEngine/ObjectBlueprint.h>

#include "AmmoAbility.h"

class MultiplicateAmmoAbility : public AmmoAbility
{
public:
	MultiplicateAmmoAbility(ni::ObjectBlueprint object, ni::ObjectTemplateBlueprint object_template, ni::TilesetBlueprint tileset, ni::GameMode* mode, bool propagate = false);

	void Activate(ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> owner_id) override;

private:
	ni::ObjectBlueprint object_blueprint_; 
	ni::ObjectTemplateBlueprint object_template_;
	ni::TilesetBlueprint tileset_;
	
	ni::GameMode* mode_ = nullptr;

	void SpawnAmmo(ni::ComponentLocator& component_locator);
};

