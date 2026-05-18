#pragma once

#include "ObstacleUpdateComponent.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/TransformComponent.h>

class MovingObstacleUpdateComponent : public ObstacleUpdateComponent
{
public:
	MovingObstacleUpdateComponent(
		ni::ComponentLocator& component_locator,
		ni::TransformComponent& transform,
		ni::Id<ni::GameObjectTag> id,
		sf::Vector2i position_movement_offset,
		sf::Vector2f movement_trigger_distance,
		sf::Vector2f collision_box_size,
		float delay_in_seconds
	);
	
	void Update() override;
	virtual void CollideTop(sf::FloatRect collision_box, ni::Id<ni::GameObjectTag> object_id) override;

private:
	sf::Vector2i movement_position_offset_ = {};
	sf::Vector2f movement_trigger_distance_;

	sf::Vector2f target_position_ = {};
	sf::Vector2f start_position_  = {};

	float amount_moved_x_ = 0;

	float    delay_in_seconds_ = 0;
	sf::Time time_since_movement_started_;

	bool moving_ = false;
	bool moved_  = false;

	void LocatePlayer();
	void TriggerMovement();
	void Move();
};

