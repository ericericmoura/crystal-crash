#pragma once

#include <string>

#include <SFML/System/Vector2.hpp>
#include <NiEngine/UpdateComponent.h>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/AnimatedGraphicsComponent.h>
#include <NiEngine/Subject.h>
#include <NiEngine/HitboxComponent.h>

#include "CharacterPhysicsComponent.h"
#include "PlatformerGameMode.h"

class PlayerUpdateComponent : public ni::UpdateComponent
{
public:
	~PlayerUpdateComponent();
	PlayerUpdateComponent(ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> owner_id, PlatformerGameMode& game_mode, sf::Vector2f player_size);

	void Init(ni::AnimatedGraphicsComponent& graphics, CharacterPhysicsComponent& physics);
	void Update() override;

	void Die();

	ni::HitboxComponent GetFeetHitbox() const { return feet_hitbox_; }

private:
	inline static const int kAnimationRow = 12;
	inline static const std::string kJumpAnimationKey = "jump";
	inline static const std::string kWalkAnimationKey = "walk";

	inline static const std::string kJumpSoundKey  = "sounds/jump.wav";
	inline static const std::string kDeathSoundKey = "sounds/death.wav";

	int key_pressed_event_id_ = 0;

	ni::Subject<> on_player_killed_;

	ni::HitboxComponent feet_hitbox_;

	bool airborne_ = false;
	bool dead_     = false;
	bool playing_dead_animation_ = false;

	int last_moved_direction_ = 1;

	void Jump();
	void FallFromPlatform();
};

