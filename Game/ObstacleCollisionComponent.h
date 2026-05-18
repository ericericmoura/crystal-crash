#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/Id.h>
#include <NiEngine/GameObjectTag.h>

#include "CharacterPhysicsComponent.h"

class ObstacleCollisionComponent
{
public:
	virtual void SolveTopCollision    (sf::FloatRect collision_box, ni::ComponentLocator& locator, ni::Id<ni::GameObjectTag> id) = 0;
	virtual void SolveBottomCollision (sf::FloatRect collision_box, ni::ComponentLocator& locator, ni::Id<ni::GameObjectTag> id) = 0;
	virtual void SolveFrontCollision  (sf::FloatRect collision_box, ni::ComponentLocator& locator, ni::Id<ni::GameObjectTag> id) = 0;
	virtual void SolveRightCollision  (sf::FloatRect collision_box, ni::ComponentLocator& locator, ni::Id<ni::GameObjectTag> id) = 0;
	virtual void SolveLeftCollision   (sf::FloatRect collision_box, ni::ComponentLocator& locator, ni::Id<ni::GameObjectTag> id) = 0;
	virtual void SolveTopCollisionLost(sf::FloatRect collision_box, ni::ComponentLocator& locator, ni::Id<ni::GameObjectTag> id) {};

protected:
	CharacterPhysicsComponent* GetCharacterPhysics(ni::ComponentLocator& locator, ni::Id<ni::GameObjectTag> id);
};
