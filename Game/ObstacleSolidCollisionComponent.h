#pragma once

#include "ObstacleCollisionComponent.h"

#include <SFML/Graphics/Rect.hpp>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>

class ObstacleSolidCollisionComponent : public ObstacleCollisionComponent
{
public:
	void SolveTopCollision    (sf::FloatRect collision_box, ni::ComponentLocator& locator, ni::Id<ni::GameObjectTag> id) override;
	void SolveBottomCollision (sf::FloatRect collision_box, ni::ComponentLocator& locator, ni::Id<ni::GameObjectTag> id) override;
	void SolveFrontCollision  (sf::FloatRect collision_box, ni::ComponentLocator& locator, ni::Id<ni::GameObjectTag> id) override;
	void SolveTopCollisionLost(sf::FloatRect collision_box, ni::ComponentLocator& locator, ni::Id<ni::GameObjectTag> id) override;

	void SolveRightCollision(sf::FloatRect collision_box, ni::ComponentLocator& locator, ni::Id<ni::GameObjectTag> id) override;
	void SolveLeftCollision(sf::FloatRect collision_box, ni::ComponentLocator& locator, ni::Id<ni::GameObjectTag> id) override;
};

