#pragma once

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
	SlingshotUpdateComponent(ni::ComponentLocator& component_locator, sf::Vector2f start_position, sf::Vector2i sprite_size);
	void Update() override;

private:
	bool is_dragging_ = false;
	
	bool loaded_ammo_queue_ = false;

	std::queue<ni::Id<ni::GameObjectTag>> ammo_queue_ = {};

	ni::Id<ni::GameObjectTag> current_ammo_id_ = {};

	sf::Vector2i mouse_start_position_   = {};
	sf::Vector2i mouse_current_position_ = {};

	sf::Vector2f initial_ammo_position_ = {};

	void Reload();
	void LoadAmmoQueue();
};

