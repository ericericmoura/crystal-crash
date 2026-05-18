#include "ObstacleCollisionComponent.h"

#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>

#include "CharacterPhysicsComponent.h"

CharacterPhysicsComponent* ObstacleCollisionComponent::GetCharacterPhysics(ni::ComponentLocator& locator, ni::Id<ni::GameObjectTag> id)
{
    return static_cast<CharacterPhysicsComponent*>(locator.GetPhysicsComponent(id));
}
