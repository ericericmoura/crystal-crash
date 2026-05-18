#include "ObstacleSolidCollisionComponent.h"

#include <SFML/Graphics/Rect.hpp>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/TransformComponent.h>

#include "CharacterPhysicsComponent.h"

void ObstacleSolidCollisionComponent::SolveTopCollision(sf::FloatRect collision_box, ni::ComponentLocator& locator, ni::Id<ni::GameObjectTag> id)
{
	ni::TransformComponent*    transform = locator.GetTransformComponent(id);
	CharacterPhysicsComponent* physics   = GetCharacterPhysics(locator, id);

	physics->CollideBottom(*transform, collision_box);
	physics->SetIsOnGround(true);
}

void ObstacleSolidCollisionComponent::SolveBottomCollision(sf::FloatRect collision_box, ni::ComponentLocator & locator, ni::Id<ni::GameObjectTag> id)
{
	ni::TransformComponent*    transform = locator.GetTransformComponent(id);
	CharacterPhysicsComponent* physics   = GetCharacterPhysics(locator, id);

	physics->CollideTop(*transform, collision_box);
}

void ObstacleSolidCollisionComponent::SolveFrontCollision(sf::FloatRect collision_box, ni::ComponentLocator & locator, ni::Id<ni::GameObjectTag> id)
{
	ni::TransformComponent*    transform = locator.GetTransformComponent(id);
	CharacterPhysicsComponent* physics   = GetCharacterPhysics(locator, id);

	physics->CollideFront(*transform, collision_box);
}

void ObstacleSolidCollisionComponent::SolveTopCollisionLost(sf::FloatRect collision_box, ni::ComponentLocator& locator, ni::Id<ni::GameObjectTag> id)
{
	ni::TransformComponent* transform = locator.GetTransformComponent(id);
	CharacterPhysicsComponent* physics = GetCharacterPhysics(locator, id);

	physics->SetIsOnGround(false);
}

void ObstacleSolidCollisionComponent::SolveRightCollision(sf::FloatRect collision_box, ni::ComponentLocator& locator, ni::Id<ni::GameObjectTag> id)
{
	ni::TransformComponent* transform = locator.GetTransformComponent(id);
	CharacterPhysicsComponent* physics = GetCharacterPhysics(locator, id);

	physics->CollideSides(*transform, collision_box, 1);
}

void ObstacleSolidCollisionComponent::SolveLeftCollision(sf::FloatRect collision_box, ni::ComponentLocator & locator, ni::Id<ni::GameObjectTag> id)
{
	ni::TransformComponent* transform = locator.GetTransformComponent(id);
	CharacterPhysicsComponent* physics = GetCharacterPhysics(locator, id);

	physics->CollideSides(*transform, collision_box, -1);
}
