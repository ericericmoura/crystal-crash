#pragma once

#include "ObstacleUpdateComponent.h"

#include <SFML/System/Vector2.hpp>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/GameMode.h>
#include <NiEngine/Subject.h>

class ExitDoorUpdateComponent : public ObstacleUpdateComponent
{
public:
	ExitDoorUpdateComponent(ni::GameMode& mode, ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> id, sf::Vector2f collision_box_size);

protected:
	virtual void CollideTop(sf::FloatRect collision_box, ni::Id<ni::GameObjectTag> object_id)    override;
	virtual void CollideBottom(sf::FloatRect collision_box, ni::Id<ni::GameObjectTag> object_id) override;
	virtual void CollideFront(sf::FloatRect collision_box, ni::Id<ni::GameObjectTag> object_id)  override;

private:
	ni::Subject<> on_player_enter_;
};

