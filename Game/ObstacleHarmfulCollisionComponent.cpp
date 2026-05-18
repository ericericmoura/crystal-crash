#include "ObstacleHarmfulCollisionComponent.h"

#include <SFML/Graphics/Rect.hpp>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/UpdateComponent.h>

#include "PlayerUpdateComponent.h"
#include "EnemyUpdateComponent.h"

void ObstacleHarmfulCollisionComponent::SolveTopCollision(sf::FloatRect collision_box, ni::ComponentLocator& locator, ni::Id<ni::GameObjectTag> id)
{
	KillCharacter(locator, id);
}

void ObstacleHarmfulCollisionComponent::SolveBottomCollision(sf::FloatRect collision_box, ni::ComponentLocator& locator, ni::Id<ni::GameObjectTag> id)
{
	KillCharacter(locator, id);
}

void ObstacleHarmfulCollisionComponent::SolveFrontCollision(sf::FloatRect collision_box, ni::ComponentLocator& locator, ni::Id<ni::GameObjectTag> id)
{
	KillCharacter(locator, id);
}

void ObstacleHarmfulCollisionComponent::KillCharacter(ni::ComponentLocator& locator, ni::Id<ni::GameObjectTag> id)
{
	ni::UpdateComponent* update = locator.GetUpdateComponent(id);

	auto player_update = dynamic_cast<PlayerUpdateComponent*>(update);
	if (player_update)
	{
		player_update->Die();
	}
	auto enemy_update = dynamic_cast<EnemyUpdateComponent*>(update);
	if (enemy_update)
	{
		enemy_update->Die();
	}
}

void ObstacleHarmfulCollisionComponent::SolveRightCollision(sf::FloatRect collision_box, ni::ComponentLocator& locator, ni::Id<ni::GameObjectTag> id)
{}

void ObstacleHarmfulCollisionComponent::SolveLeftCollision(sf::FloatRect collision_box, ni::ComponentLocator & locator, ni::Id<ni::GameObjectTag> id)
{}
