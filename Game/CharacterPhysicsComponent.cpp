#include "CharacterPhysicsComponent.h"

#include <cstdlib>

#include <SFML/System/Vector2.hpp>
#include <NiEngine/TransformComponent.h>
#include <NiEngine/Tilemap.h>
#include <NiEngine/MathUtility.h>

CharacterPhysicsComponent::CharacterPhysicsComponent(sf::Vector2i character_size) : size_(character_size)
{
}

void CharacterPhysicsComponent::PhysicsUpdate(ni::TransformComponent& transform_component, const ni::Tilemap& current_tilemap, float delta)
{
	if (state_ == CharacterState::Falling || state_ == CharacterState::Jumping)
	{
		velocity_.y += gravity_;

		if (velocity_.y > 0 && state_ != CharacterState::Falling)
		{
			state_ = CharacterState::Falling;
			on_falling_.Notify();
		}
	}
	else
	{
		velocity_.y = 0;	
	}

	sf::Vector2f new_scale = transform_component.GetTransformable().getScale();
	
	int movement_sign = ni::MathUtility::GetSign(velocity_.x);
	if (movement_sign != 0)
	{
		new_scale.x = abs(new_scale.x) * movement_sign;
	}

	transform_component.GetTransformable().setScale(new_scale);
	transform_component.GetTransformable().move(velocity_ * delta);

	HandleCollisions(transform_component, current_tilemap);
}

void CharacterPhysicsComponent::Move(float dir)
{
	velocity_.x = dir * speed_;

	if (dir != 0 && state_ != CharacterState::Falling && state_ != CharacterState::Jumping)
	{
		state_ = CharacterState::Moving;
	}
}

void CharacterPhysicsComponent::Jump()
{
	if (state_ == CharacterState::Falling || state_ == CharacterState::Jumping)
	{
		return;
	}
	ForceJump();
}

void CharacterPhysicsComponent::ForceJump()
{
	velocity_.y = -jump_force_;
	state_ = CharacterState::Jumping;

	on_jumping_.Notify();
}

void CharacterPhysicsComponent::FallFromPlatform()
{
	fall_through_platform_ = true;
}
