#include "WalkerEnemyUpdateComponent.h"

#include <iostream>

#include <SFML/System/Vector2.hpp>
#include <NiEngine/AnimatedGraphicsComponent.h>
#include <NiEngine/Animation.h>
#include <NiEngine/UpdateComponent.h>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>

#include "CharacterPhysicsComponent.h"
#include "PlatformerGameMode.h"
#include "PlayerUpdateComponent.h"
#include "EnemyUpdateComponent.h"

WalkerEnemyUpdateComponent::WalkerEnemyUpdateComponent(ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> owner_id, int start_movement_direction, sf::Vector2f enemy_size)
	: EnemyUpdateComponent(component_locator, owner_id, enemy_size)
{
	movement_direction = start_movement_direction;	
}

void WalkerEnemyUpdateComponent::Init(ni::AnimatedGraphicsComponent& graphics, CharacterPhysicsComponent& physics)
{
	physics.OnCollideLeft([this]() {
		std::cout << "\ncollide left!";
		movement_direction =  1;
	});
	physics.OnCollideRight([this]() {
		std::cout << "\ncollide right!";
		movement_direction = -1;
	});

	physics.OnLanding([this]() {
		if (is_dead_)
		{
			played_death_animation_ = true;
		}
	});

	ni::Animation walk_animation;
	walk_animation.key_ = kWalkAnimationKey;
	walk_animation.start_frame = 1;
	walk_animation.frame_count = 2;
	walk_animation.animation_row = kAnimationRow;

	graphics.RegisterAnimation(walk_animation);
}

void WalkerEnemyUpdateComponent::Update()
{
	auto graphics = component_locator_.GetFirstAnimatedGraphicsComponent(owner_id_);
	auto physics  = static_cast<CharacterPhysicsComponent*>(component_locator_.GetPhysicsComponent(owner_id_));
	if (is_dead_)
	{
		graphics->SetFrame(kAnimationRow, 4);

		if (!played_death_animation_)
		{
			PlayKnockbackAnimation(*physics, -movement_direction);
		}
		else
		{
			physics->Move(0);
		}
		return;
	}
	UpdatePlayerTrackingStatus();
	if (distance_to_player_ < 32.0f)
	{
		EnemyUpdateComponent::Update();
	}	
	if (physics)
	{
		physics->Move(movement_direction);
	}
	graphics->Play(kWalkAnimationKey, .1, true);
}

void WalkerEnemyUpdateComponent::CollideSides()
{	
	KillPlayer();
}

void WalkerEnemyUpdateComponent::CollideTop()
{
	auto player_physics = static_cast<CharacterPhysicsComponent*>(component_locator_.GetPhysicsComponent(player_id_));

	if (player_physics->GetVelocity().y < 0)
	{
		return;
	}
	is_dead_ = true;

	float old_player_jump_force = player_physics->GetJumpForce();
	player_physics->SetJumpForce(old_player_jump_force + old_player_jump_force * .3f);
	player_physics->ForceJump();
	player_physics->SetJumpForce(old_player_jump_force);
}

void WalkerEnemyUpdateComponent::CollideBottom()
{
	KillPlayer();
}