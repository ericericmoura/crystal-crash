#include "EnemyUpdateComponent.h"

#include <cstdlib>

#include <SFML/System/Vector2.hpp>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/UpdateComponent.h>
#include <NiEngine/TransformComponent.h>
#include <NiEngine/HitboxComponent.h>

#include "PlatformerGameMode.h"
#include "CharacterPhysicsComponent.h"
#include "PlayerUpdateComponent.h"

EnemyUpdateComponent::EnemyUpdateComponent(ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> owner_id, sf::Vector2f enemy_size)
	: ni::UpdateComponent(component_locator)
{
	owner_id_ = owner_id;

	head_hurtbox_.SetSize({ enemy_size.x, enemy_size.y / 2.0f });
	head_hurtbox_.SetPositionOffset({ -enemy_size.x/2.0f, -enemy_size.y/2.0f - 1 });
}

void EnemyUpdateComponent::Update()
{
	CheckCollisionWithPlayer();
}

void EnemyUpdateComponent::CheckCollisionWithPlayer()
{
	if (is_dead_)
	{
		return;
	}
	if (player_id_.id_ == -1)
	{
		player_id_ = component_locator_.GetIdByTag(PlatformerGameMode::kPlayerTag);
	}
	auto physics        = static_cast<CharacterPhysicsComponent*>(component_locator_.GetPhysicsComponent(owner_id_ ));
	auto player_physics = static_cast<CharacterPhysicsComponent*>(component_locator_.GetPhysicsComponent(player_id_));
	
	ni::TransformComponent* transform        = component_locator_.GetTransformComponent(owner_id_);
	ni::TransformComponent* player_transform = component_locator_.GetTransformComponent(player_id_);	

	if (physics->GetFeetBounds(transform->GetTransformable().getPosition()).findIntersection(player_physics->GetHeadBounds(player_transform->GetTransformable().getPosition())))
	{
		CollideBottom();
	}
	if (physics->GetSideBounds(transform->GetTransformable().getPosition(),  1).findIntersection(player_physics->GetSideBounds(player_transform->GetTransformable().getPosition(), -1)) ||
		physics->GetSideBounds(transform->GetTransformable().getPosition(), -1).findIntersection(player_physics->GetSideBounds(player_transform->GetTransformable().getPosition(), 1)))
	{
		CollideSides();
	}
	ni::HitboxComponent player_hitbox = static_cast<PlayerUpdateComponent*>(component_locator_.GetUpdateComponent(player_id_))->GetFeetHitbox();

	head_hurtbox_.SetPosition(transform->GetTransformable().getPosition());
	if (head_hurtbox_.IsColliding(player_hitbox))
	{
		CollideTop();
	}
}

void EnemyUpdateComponent::UpdatePlayerTrackingStatus()
{
	if (player_id_.id_ == -1)
	{
		player_id_ = component_locator_.GetIdByTag(PlatformerGameMode::kPlayerTag);
	}
	ni::TransformComponent* transform        = component_locator_.GetTransformComponent(owner_id_);
	ni::TransformComponent* player_transform = component_locator_.GetTransformComponent(player_id_);

	direction_to_player_ = player_transform->GetTransformable().getPosition() - transform->GetTransformable().getPosition();
	horizontal_distance_to_player_ = std::abs(direction_to_player_.x);
	distance_to_player_  = direction_to_player_.length();

	if (direction_to_player_.length() == 0)
	{
		return;
	}
	direction_to_player_ = direction_to_player_.normalized();
}

void EnemyUpdateComponent::PlayKnockbackAnimation(CharacterPhysicsComponent& physics, int direction)
{
	if (!airborne_)
	{
		float old_jump_force = physics.GetJumpForce();
		physics.SetJumpForce(150);
		physics.ForceJump();
		physics.SetJumpForce(old_jump_force);
		airborne_ = true;
	}
	physics.Move(direction);
}

void EnemyUpdateComponent::KillPlayer()
{
	if (player_id_.id_ == -1)
	{
		player_id_ = component_locator_.GetIdByTag(PlatformerGameMode::kPlayerTag);
	}
	ni::UpdateComponent* update = component_locator_.GetUpdateComponent(player_id_);
	auto player_update = static_cast<PlayerUpdateComponent*>(update);
	player_update->Die();
}
