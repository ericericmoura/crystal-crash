#include "ObstacleUpdateComponent.h"

#include <memory>
#include <utility>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/UpdateComponent.h>
#include <NiEngine/TransformComponent.h>

#include "CharacterPhysicsComponent.h"
#include "ObstacleCollisionComponent.h"
#include "PlatformerGameMode.h"
#include <vector>

ObstacleUpdateComponent::ObstacleUpdateComponent(ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> id, sf::Vector2f collision_box_size) 
	: ni::UpdateComponent(component_locator)
{
	owner_id_ = id;
	collision_box_size_ = collision_box_size;	
}

void ObstacleUpdateComponent::RegisterCollisionComponent(std::unique_ptr<ObstacleCollisionComponent> collision_component)
{
	collision_components_.push_back(std::move(collision_component));
}

void ObstacleUpdateComponent::Update()
{
	HandleCollisions();
}

void ObstacleUpdateComponent::CollideTop(sf::FloatRect collision_box, ni::Id<ni::GameObjectTag> object_id)
{
	for (auto& collision : collision_components_)
	{
		collision->SolveTopCollision(collision_box, component_locator_, object_id);
	}
}

void ObstacleUpdateComponent::CollideBottom(sf::FloatRect collision_box, ni::Id<ni::GameObjectTag> object_id)
{
	for (auto& collision : collision_components_)
	{
		collision->SolveBottomCollision(collision_box, component_locator_, object_id);
	}
}

void ObstacleUpdateComponent::CollideFront(sf::FloatRect collision_box, ni::Id<ni::GameObjectTag> object_id)
{
	for (auto& collision : collision_components_)
	{
		collision->SolveFrontCollision(collision_box, component_locator_, object_id);
	}
}

void ObstacleUpdateComponent::CollideLeft(sf::FloatRect collision_box, ni::Id<ni::GameObjectTag> object_id)
{
	for (auto& collision : collision_components_)
	{
		collision->SolveLeftCollision(collision_box, component_locator_, object_id);
	}
}

void ObstacleUpdateComponent::CollideRight(sf::FloatRect collision_box, ni::Id<ni::GameObjectTag> object_id)
{
	for (auto& collision : collision_components_)
	{
		collision->SolveRightCollision(collision_box, component_locator_, object_id);
	}
}

void ObstacleUpdateComponent::HandleCollisions()
{
	if (player_id_.id_ == -1)
	{
		player_id_ = component_locator_.GetIdByTag(PlatformerGameMode::kPlayerTag);
	}

	ni::TransformComponent* transform = component_locator_.GetTransformComponent(owner_id_);

	sf::FloatRect collision_box;
	collision_box.position = transform->GetTransformable().getPosition();
	collision_box.size     = collision_box_size_;	

	sf::Vector2f box_center_position = collision_box.position;
	box_center_position += collision_box.size / 2.0f;

	HandleCollisionById(player_id_, collision_box, box_center_position);

	std::vector<ni::Id<ni::GameObjectTag>> enemies = component_locator_.GetIdsByTag(PlatformerGameMode::kEnemyTag);
	if (enemies.empty())
	{
		return;
	}
	for (auto& enemy_id : enemies)
	{
		HandleCollisionById(enemy_id, collision_box, box_center_position);
	}
}

void ObstacleUpdateComponent::HandleCollisionById(ni::Id<ni::GameObjectTag> object_id, const sf::FloatRect& collision_box, const sf::Vector2f& collision_box_center_position)
{
	ni::TransformComponent* other_transform  = component_locator_.GetTransformComponent(object_id);

	float distance = (other_transform->GetTransformable().getPosition() - collision_box_center_position).length();

	if (distance > collision_box.size.x && distance > collision_box.size.y)
	{
		return;
	}
	
	auto other_physics = static_cast<CharacterPhysicsComponent*>(component_locator_.GetPhysicsComponent(object_id));

	if (collision_box.findIntersection(other_physics->GetFeetBounds(other_transform->GetTransformable().getPosition())))
	{
		CollideTop(collision_box, object_id);

		colliding_ = true;
	}
	else if (colliding_)
	{
		for (auto& collision : collision_components_)
		{
			collision->SolveTopCollisionLost(collision_box, component_locator_, object_id);
		}
		colliding_ = false;
	}

	if (collision_box.findIntersection(other_physics->GetFrontBounds(other_transform->GetTransformable().getPosition())))
	{
		CollideFront(collision_box, object_id);
	}
	else if (collision_box.findIntersection(other_physics->GetSideBounds(other_transform->GetTransformable().getPosition(), -1)))
	{
		CollideLeft(collision_box, object_id);
	}
	else if (collision_box.findIntersection(other_physics->GetSideBounds(other_transform->GetTransformable().getPosition(), 1)))
	{
		CollideRight(collision_box, object_id);
	}
	if (collision_box.findIntersection(other_physics->GetHeadBounds(other_transform->GetTransformable().getPosition())))
	{
		CollideBottom(collision_box, object_id);
	}
}
