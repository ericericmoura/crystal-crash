#pragma once

#include <string>

#include <SFML/System/Vector2.hpp>
#include <NiEngine/AnimatedGraphicsComponent.h>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>

#include "EnemyUpdateComponent.h"
#include "CharacterPhysicsComponent.h"

class WalkerEnemyUpdateComponent : public EnemyUpdateComponent
{
public:
	WalkerEnemyUpdateComponent(ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> owner_id, int start_movement_direction, sf::Vector2f enemy_size);
	void Init(ni::AnimatedGraphicsComponent& graphics, CharacterPhysicsComponent& physics) override;
	void Update() override;

private:
	inline static const int kAnimationRow = 17;
	inline static const std::string kWalkAnimationKey = "walk";
	
	int movement_direction = 1;

	void CollideSides() override;
	void CollideTop() override;
	void CollideBottom() override;
};

