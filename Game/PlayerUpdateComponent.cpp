#include "PlayerUpdateComponent.h"

#include <iostream>

#include <NiEngine/TransformComponent.h>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/UpdateComponent.h>
#include <NiEngine/ServiceLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/SoundEngine.h>
#include <NiEngine/Animation.h>
#include <NiEngine/AnimatedGraphicsComponent.h>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "CharacterPhysicsComponent.h"
#include "PlatformerGameMode.h"

PlayerUpdateComponent::~PlayerUpdateComponent()
{
	ni::ServiceLocator::Instance().GetEventDispatcher().RemoveKeyPressedEvent(key_pressed_event_id_);
}

PlayerUpdateComponent::PlayerUpdateComponent(ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> owner_id, PlatformerGameMode& game_mode, sf::Vector2f player_size)
	: ni::UpdateComponent(component_locator)
{
	owner_id_ = owner_id;

	ni::ServiceLocator::Instance().GetSoundEngine().Preload(kJumpSoundKey);
	ni::ServiceLocator::Instance().GetSoundEngine().Preload(kDeathSoundKey);

	on_player_killed_.Subscribe([&game_mode]() {
		game_mode.RestartLevel();
	});

	feet_hitbox_.SetSize({ player_size.x / 2.0f, player_size.y / 2.0f + 1 });
	feet_hitbox_.SetPositionOffset({ -player_size.x/4.0f,   0 });
}

void PlayerUpdateComponent::Init(ni::AnimatedGraphicsComponent& graphics, CharacterPhysicsComponent& physics)
{
	key_pressed_event_id_ = ni::ServiceLocator::Instance().GetEventDispatcher().OnKeyPressed([this](const sf::Event::KeyPressed& event) {
		if (event.scancode == sf::Keyboard::Scancode::Space)
		{
			Jump();
		}
		else if (event.scancode == sf::Keyboard::Scancode::S)
		{
			FallFromPlatform();
		}
	});

	physics.OnFalling([this]() {
		if (dead_)
		{
			return;
		}
		auto graphics = component_locator_.GetFirstAnimatedGraphicsComponent(owner_id_);
		graphics->SetFrame(kAnimationRow, 4);
	});

	physics.OnJumping([this]() {
		if (dead_)
		{
			return;
		}
		auto graphics = component_locator_.GetFirstAnimatedGraphicsComponent(owner_id_);
		graphics->SetFrame(kAnimationRow, 3);

		airborne_ = true;

		ni::ServiceLocator::Instance().GetSoundEngine().PlaySound(kJumpSoundKey, 1);
	});

	physics.OnLanding([this]() {
		airborne_ = false;
		if (dead_)
		{
			playing_dead_animation_ = false;
		}
	});

	ni::Animation jump_animation;
	jump_animation.key_ = kJumpAnimationKey;
	jump_animation.start_frame = 3;
	jump_animation.frame_count = 2;
	jump_animation.animation_row = 12;

	ni::Animation walk_animation;
	walk_animation.key_ = kWalkAnimationKey;
	walk_animation.start_frame = 1;
	walk_animation.frame_count = 2;
	walk_animation.animation_row = 12;

	graphics.RegisterAnimation(jump_animation);
	graphics.RegisterAnimation(walk_animation);
}

void PlayerUpdateComponent::Update()
{
	ni::TransformComponent* transform = component_locator_.GetTransformComponent(owner_id_);
	feet_hitbox_.SetPosition(transform->GetTransformable().getPosition());

	auto physics = static_cast<CharacterPhysicsComponent*>(component_locator_.GetPhysicsComponent(owner_id_));

	if (dead_)
	{
		if (playing_dead_animation_)
		{
			physics->Move(-last_moved_direction_);
		}
		else
		{
			physics->Move(0);
		}
		return;
	}
	
	int dir = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D) - sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A);
	last_moved_direction_ = dir;

	if (physics)
	{
		physics->Move(dir);
	}
	auto graphics = component_locator_.GetFirstAnimatedGraphicsComponent(owner_id_);
	if (!graphics || airborne_)
	{		
		return;
	}
	if (dir == 0)
	{
		graphics->SetFrame(kAnimationRow, 0);
		return;
	}
	graphics->Play(kWalkAnimationKey, .1, true);
}

void PlayerUpdateComponent::Die()
{
	if (dead_)
	{
		return;
	}
	ni::ServiceLocator::Instance().GetSoundEngine().PlaySound(kDeathSoundKey, 1.5f);
	dead_ = true;
	auto physics = static_cast<CharacterPhysicsComponent*>(component_locator_.GetPhysicsComponent(owner_id_));
	if (physics)
	{	
		physics->SetJumpForce(150);
		physics->ForceJump();
		airborne_ = true;
		playing_dead_animation_ = true;
	}
	auto graphics = component_locator_.GetFirstAnimatedGraphicsComponent(owner_id_);
	if (graphics)
	{
		graphics->SetFrame(kAnimationRow, 6);
	}
	on_player_killed_.Notify();
}

void PlayerUpdateComponent::Jump()
{
	if (dead_)
	{
		return;
	}
	auto physics = static_cast<CharacterPhysicsComponent*>(component_locator_.GetPhysicsComponent(owner_id_));
	if (physics)
	{
		physics->Jump();
	}
}

void PlayerUpdateComponent::FallFromPlatform()
{
	auto physics = static_cast<CharacterPhysicsComponent*>(component_locator_.GetPhysicsComponent(owner_id_));
	if (physics)
	{
		physics->FallFromPlatform();
	}
}
