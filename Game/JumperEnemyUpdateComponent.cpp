#include "JumperEnemyUpdateComponent.h"

#include <SFML/System/Vector2.hpp>
#include <NiEngine/AnimatedGraphicsComponent.h>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/MathUtility.h>
#include <NiEngine/Engine.h>

#include "EnemyUpdateComponent.h"
#include "CharacterPhysicsComponent.h"

JumperEnemyUpdateComponent::JumperEnemyUpdateComponent(ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> owner_id, sf::Vector2f enemy_size, float jump_cooldown, float upside_down_cooldown)
	: EnemyUpdateComponent(component_locator, owner_id, enemy_size)
{
	jump_cooldown_        = jump_cooldown;
	upside_down_cooldown_ = upside_down_cooldown;
}

void JumperEnemyUpdateComponent::Init(ni::AnimatedGraphicsComponent& graphics, CharacterPhysicsComponent& physics)
{
	physics.OnLanding([this]() {
		airborne_ = false;

		auto physics = static_cast<CharacterPhysicsComponent*>(component_locator_.GetPhysicsComponent(owner_id_));
		physics->Move(0);

		if (upside_down_)
		{
			played_death_animation_ = true;
			return;
		}
		auto graphics = component_locator_.GetFirstAnimatedGraphicsComponent(owner_id_);
		graphics->SetFrame(kAnimationRow, 4);
	});

	physics.OnJumping([this]() {
		airborne_ = true;
		if (upside_down_)
		{
			return;
		}
		auto graphics = component_locator_.GetFirstAnimatedGraphicsComponent(owner_id_);
		graphics->SetFrame(kAnimationRow, 3);
	});
}

void JumperEnemyUpdateComponent::Update()
{
	auto physics = static_cast<CharacterPhysicsComponent*>(component_locator_.GetPhysicsComponent(owner_id_));

	if (dead_)
	{
		return;
	}

	UpdatePlayerTrackingStatus();
	if (distance_to_player_ < 32.0f)
	{
		EnemyUpdateComponent::Update();
	}

	if (upside_down_)
	{
		float time_elapsed_since_upside_down = (ni::Engine::time_elapsed - time_since_upside_down_).asSeconds();
		if (time_elapsed_since_upside_down > upside_down_cooldown_)
		{
			upside_down_ = false;

			float old_jump_force = physics->GetJumpForce();
			physics->SetJumpForce(150);
			physics->Jump();
			physics->SetJumpForce(old_jump_force);
			last_time_since_jumping_ = ni::Engine::time_elapsed;
		}

		if (!played_death_animation_)
		{
			PlayKnockbackAnimation(*physics, -last_facing_direction_);
		}
		return;
	}
	else
	{
		float time_elapsed_since_last_jump = (ni::Engine::time_elapsed - last_time_since_jumping_).asSeconds();
		if (time_elapsed_since_last_jump > jump_cooldown_)
		{
			physics->Jump();
			last_time_since_jumping_ = ni::Engine::time_elapsed;
		}
		if (airborne_ && horizontal_distance_to_player_ > 2.0f)
		{
			last_facing_direction_ = ni::MathUtility::GetSign(direction_to_player_.x);
			physics->Move(last_facing_direction_);
		}
		else
		{
			physics->Move(0);
		}
	}
}

void JumperEnemyUpdateComponent::CollideSides()
{
	if (upside_down_)
	{
		return;
	}
	KillPlayer();
}

void JumperEnemyUpdateComponent::CollideTop()
{
	auto player_physics = static_cast<CharacterPhysicsComponent*>(component_locator_.GetPhysicsComponent(player_id_));

	if (player_physics->GetVelocity().y < 0)
	{
		return;
	}
	float old_player_jump_force = player_physics->GetJumpForce();
	player_physics->SetJumpForce(old_player_jump_force + old_player_jump_force * .3f);
	player_physics->ForceJump();
	player_physics->SetJumpForce(old_player_jump_force);

	auto graphics = component_locator_.GetFirstAnimatedGraphicsComponent(owner_id_);
	if (!upside_down_)
	{
		graphics->SetFrame(kAnimationRow, 6);
		upside_down_ = true;

		time_since_upside_down_ = ni::Engine::time_elapsed;
		return;
	}
	graphics->SetFrame(kAnimationRow, 5);
	dead_ = true;
}

void JumperEnemyUpdateComponent::CollideBottom()
{
	if (upside_down_)
	{
		return;
	}
	KillPlayer();
	auto physics = static_cast<CharacterPhysicsComponent*>(component_locator_.GetPhysicsComponent(owner_id_));
	float old_player_jump_force = physics->GetJumpForce();
	physics->SetJumpForce(old_player_jump_force + old_player_jump_force * .3f);
	physics->ForceJump();
	physics->SetJumpForce(old_player_jump_force);
}
