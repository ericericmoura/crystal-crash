#pragma once

#include <functional>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <NiEngine/TransformComponent.h>
#include <NiEngine/PhysicsComponent.h>
#include <NiEngine/Tilemap.h>
#include <NiEngine/Subject.h>
#include <NiEngine/TileBlueprint.h>

enum CharacterState
{
	Jumping,
	Falling,
	Idle,
	Moving
};

class CharacterPhysicsComponent : public ni::PhysicsComponent
{
public:	
	CharacterPhysicsComponent(sf::Vector2i character_size);
	void PhysicsUpdate(ni::TransformComponent& transform_component, const ni::Tilemap& current_tilemap, float delta) override;

	void Move(float dir);
	void Jump();
	void ForceJump();
	void FallFromPlatform();
	sf::Vector2f GetVelocity() const { return velocity_;  };

	float GetJumpForce() const { return jump_force_; }
	void SetJumpForce(float new_value) { jump_force_ = new_value; }
	void SetSpeed    (float new_value) { speed_      = new_value; }
	void SetGravity  (float new_value) { gravity_    = new_value; }

	int OnFalling(std::function<void()> callback) { return on_falling_.Subscribe(callback); }
	int OnJumping(std::function<void()> callback) { return on_jumping_.Subscribe(callback); }
	int OnLanding(std::function<void()> callback) { return on_landing_.Subscribe(callback); }

	int OnCollideLeft (std::function<void()> callback) { return on_collide_left_ .Subscribe(callback); }
	int OnCollideRight(std::function<void()> callback) { return on_collide_right_.Subscribe(callback); }

	sf::FloatRect GetFeetBounds (sf::Vector2f position) const;
	sf::FloatRect GetHeadBounds (sf::Vector2f position) const;
	sf::FloatRect GetFrontBounds(sf::Vector2f position) const;
	sf::FloatRect GetSideBounds (sf::Vector2f position, int sign) const;

	void CollideTop    (ni::TransformComponent& transform_component, const sf::FloatRect& collision_block);
	void CollideBottom (ni::TransformComponent& transform_component, const sf::FloatRect& collision_block);
	void CollideFront  (ni::TransformComponent& transform_component, const sf::FloatRect& collision_block);
	void CollideSides  (ni::TransformComponent& transform_component, const sf::FloatRect& collision_block, int sign);

	void SetIsOnGround(bool is_on_non_tile_ground);

private:
	ni::Subject<> on_falling_;
	ni::Subject<> on_jumping_;
	ni::Subject<> on_landing_;

	ni::Subject<> on_collide_left_ ;
	ni::Subject<> on_collide_right_;

	sf::Vector2f velocity_;
	sf::Vector2i size_;

	float speed_      = 100.0f;
	float jump_force_ = 400.0f;
	float gravity_    = 20.0f;
	
	CharacterState state_ = CharacterState::Falling;

	bool fall_through_platform_    = false;
	bool falling_through_platform_ = false;
	bool is_on_ground_          = false;
	bool is_on_non_tile_ground_ = false;
	
	void HandleCollisions(ni::TransformComponent& transform_component, const ni::Tilemap& current_tilemap);

	bool TryCollideTop   (ni::TransformComponent& transform_component, const ni::TileBlueprint& tile, const sf::FloatRect& collision_block);
	bool TryCollideBottom(ni::TransformComponent& transform_component, const ni::TileBlueprint& tile, const sf::FloatRect& collision_block);
	bool TryCollideSides (ni::TransformComponent& transform_component, const ni::TileBlueprint& tile, const sf::FloatRect& collision_block);
};

