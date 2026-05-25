#pragma once

#include <math_functions.h>

#include <NiEngine/UpdateComponent.h>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>

class AmmoUpdateComponent : public ni::UpdateComponent
{
public:
	AmmoUpdateComponent(ni::Id<ni::GameObjectTag> owner_id, ni::ComponentLocator& component_locator);

	void Launch(b2Vec2 direction, float impulse);

	void Update() override;

};

