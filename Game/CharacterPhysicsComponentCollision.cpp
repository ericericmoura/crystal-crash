#include "CharacterPhysicsComponent.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <NiEngine/TransformComponent.h>
#include <NiEngine/Tilemap.h>
#include <NiEngine/MathUtility.h>
#include <NiEngine/TileBlueprint.h>
#include <NiEngine/Level.h>

void CharacterPhysicsComponent::HandleCollisions(ni::TransformComponent& transform_component, const ni::Tilemap& current_tilemap)
{
	sf::Vector2f character_position = transform_component.GetTransformable().getPosition();

	sf::FloatRect collision_area;
	collision_area.position.x = character_position.x - size_.x / 2.0f;
	collision_area.position.y = character_position.y - size_.y / 2.0f;
	collision_area.size = sf::Vector2f(size_);

	sf::FloatRect collision_block;
	collision_block.size = sf::Vector2f(size_);

	int start_x = collision_area.position.x / size_.x - 1;
	int start_y = collision_area.position.y / size_.y - 1;
	int end_x   = collision_area.position.x / size_.x + 1;
	int end_y   = collision_area.position.y / size_.y + 1;

	is_on_ground_ = false;
	for (int x = start_x; x <= end_x; ++x)
	{
		for (int y = start_y; y <= end_y; ++y)
		{
			if (current_tilemap.IsTileEmpty({ x, y }))
			{
				continue;
			}

			collision_block.position.x = x * size_.x;
			collision_block.position.y = y * size_.y;

			ni::TileBlueprint tile = current_tilemap.GetTileInfo({ x, y }, ni::Level::kTerrainLayerName);

			TryCollideTop(transform_component, tile, collision_block);

			if (fall_through_platform_ && tile.one_sided_collision_)
			{
				falling_through_platform_ = true;
				fall_through_platform_    = false;
			}

			is_on_ground_ |= TryCollideBottom(transform_component, tile, collision_block);
			TryCollideSides(transform_component, tile, collision_block);
		}
	}
	fall_through_platform_ = false;

	if ((!is_on_ground_ && !is_on_non_tile_ground_) && state_ != CharacterState::Falling)
	{
		state_ = CharacterState::Falling;
		on_falling_.Notify();
	}
}

bool CharacterPhysicsComponent::TryCollideTop(ni::TransformComponent& transform_component, const ni::TileBlueprint& tile, const sf::FloatRect& collision_block)
{
	if (!collision_block.findIntersection(GetHeadBounds(transform_component.GetTransformable().getPosition())))
	{
		return false;
	}
	if (tile.one_sided_collision_)
	{
		if (state_ == CharacterState::Falling)
		{
			falling_through_platform_ = false;
		}
		return false;
	}
	CollideTop(transform_component, collision_block);
	return true;
}

bool CharacterPhysicsComponent::TryCollideBottom(ni::TransformComponent& transform_component, const ni::TileBlueprint& tile, const sf::FloatRect& collision_block)
{
	bool pass_through    = tile.one_sided_collision_ && velocity_.y < 0;
	bool below_the_block = GetFeetBounds(transform_component.GetTransformable().getPosition()).position.y > collision_block.position.y + collision_block.size.y / 3.0f;
	if (!collision_block.findIntersection(GetFeetBounds(transform_component.GetTransformable().getPosition())) || below_the_block || pass_through || falling_through_platform_)
	{
		return false;
	}
	CollideBottom(transform_component, collision_block);

	return true;
}

bool CharacterPhysicsComponent::TryCollideSides(ni::TransformComponent& transform_component, const ni::TileBlueprint& tile, const sf::FloatRect& collision_block)
{
	bool left_collision  = static_cast<bool>(collision_block.findIntersection(GetSideBounds(transform_component.GetTransformable().getPosition(), -1)));
	bool right_collision = static_cast<bool>(collision_block.findIntersection(GetSideBounds(transform_component.GetTransformable().getPosition(),  1)));
	if ((!left_collision && !right_collision) || tile.one_sided_collision_)
	{
		return false;
	}
	if (left_collision)
	{
		CollideSides(transform_component, collision_block, -1);
	}
	if (right_collision)
	{
		CollideSides(transform_component, collision_block,  1);
	}
	return true;
}

sf::FloatRect CharacterPhysicsComponent::GetFeetBounds(sf::Vector2f position) const
{
	sf::FloatRect feet_bounds;
	feet_bounds.size.x = size_.x * 2.0f / 5.0f;
	feet_bounds.size.y = size_.y / 4.0f + 1;

	feet_bounds.position = position;
	feet_bounds.position.x -= size_.x / 5.0f;
	feet_bounds.position.y += size_.y / 4.0f;

	return feet_bounds;
}

sf::FloatRect CharacterPhysicsComponent::GetHeadBounds(sf::Vector2f position) const
{
	sf::FloatRect head_bounds;
	head_bounds.size.x = size_.x * 2.0f / 5.0f;
	head_bounds.size.y = size_.y / 4.0f;

	head_bounds.position = position;
	head_bounds.position.x -= size_.x / 5.0f;
	head_bounds.position.y -= size_.y / 2.0f;

	return head_bounds;
}

sf::FloatRect CharacterPhysicsComponent::GetFrontBounds(sf::Vector2f position) const
{
	int movement_sign = ni::MathUtility::GetSign(velocity_.x);

	return GetSideBounds(position, movement_sign);
}

sf::FloatRect CharacterPhysicsComponent::GetSideBounds(sf::Vector2f position, int sign) const
{
	float width = size_.x / 4.0f + 1;

	sf::FloatRect front_bounds;
	front_bounds.size.x = width;
	front_bounds.size.y = size_.y / 2.0f;

	front_bounds.position = position;
	front_bounds.position.x += size_.x / 4.0f * sign - (sign < 0 ? width : 0);
	front_bounds.position.y -= size_.y / 4.0f;

	return front_bounds;
}

void CharacterPhysicsComponent::CollideTop(ni::TransformComponent& transform_component, const sf::FloatRect& collision_block)
{
	velocity_.y = 0;

	sf::Vector2f snap_position = transform_component.GetTransformable().getPosition();
	snap_position.y = collision_block.position.y + collision_block.size.y + size_.y / 2.0f + 1;

	transform_component.GetTransformable().setPosition(snap_position);
}

void CharacterPhysicsComponent::CollideBottom(ni::TransformComponent& transform_component, const sf::FloatRect& collision_block)
{	
	is_on_ground_ = true;

	sf::Vector2f snap_position = transform_component.GetTransformable().getPosition();
	snap_position.y = collision_block.position.y - size_.y / 2.0f;

	transform_component.GetTransformable().setPosition(snap_position);

	if (state_ == CharacterState::Falling)
	{
		on_landing_.Notify();
		state_ = CharacterState::Idle;
	}
}

void CharacterPhysicsComponent::CollideFront(ni::TransformComponent& transform_component, const sf::FloatRect& collision_block)
{
	int movement_sign = ni::MathUtility::GetSign(velocity_.x);

	CollideSides(transform_component, collision_block, movement_sign);
}

void CharacterPhysicsComponent::CollideSides(ni::TransformComponent& transform_component, const sf::FloatRect& collision_block, int sign)
{
	sf::Vector2f snap_position = transform_component.GetTransformable().getPosition();

	float block_side = collision_block.position.x + collision_block.size.x / 2.0f;
	block_side += collision_block.size.x / 2.0f * -sign;

	const float separation = 1.0f; 
	snap_position.x = block_side + (size_.x / 2.0f + separation) * -sign;

	transform_component.GetTransformable().setPosition(snap_position);

	if (sign == -1)
	{
		on_collide_left_.Notify();
	}
	else
	{
		on_collide_right_.Notify();
	}
}

void CharacterPhysicsComponent::SetIsOnGround(bool is_on_non_tile_ground)
{
	is_on_non_tile_ground_ = is_on_non_tile_ground;
}
