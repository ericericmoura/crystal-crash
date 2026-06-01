#pragma once

#include <math_functions.h>

#include <queue>

#include <SFML/System/Vector2.hpp>
#include <NiEngine/UpdateComponent.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>

namespace ni 
{

class ComponentLocator;

}

class SlingshotUpdateComponent : public ni::UpdateComponent
{
public:
	SlingshotUpdateComponent(ni::ComponentLocator& component_locator, sf::Vector2f start_position, sf::Vector2i sprite_size, ni::Id<ni::GameObjectTag> chain_id);
	void Update() override;

private:
	inline static const float kMaxDragDistance  = 150.0f;
	inline static const float kMinMouseDistance = 20.0f;

	bool is_dragging_       = false;
	bool loaded_ammo_queue_ = false;

	bool out_of_ammo_ = false;
	bool launched_last_ammo_ = false;

	ni::Id<ni::GameObjectTag> chain_id_ = {};

	std::queue<ni::Id<ni::GameObjectTag>> ammo_queue_ = {};

	ni::Id<ni::GameObjectTag> current_ammo_id_ = {};

	sf::Vector2f mouse_start_position_   = {};
	sf::Vector2f mouse_current_position_ = {};

	sf::Vector2f initial_ammo_position_ = {};

	float  drag_distance_     = 0.0f;
	b2Vec2 drag_direction_ = {};

	void Reload();
	void LoadAmmoQueue();
};

