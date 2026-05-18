#pragma once

#include <SFML/System/Vector2.hpp>
#include <NiEngine/UpdateComponent.h>

namespace ni 
{

class ComponentLocator;

}

class SlingshotUpdateComponent : public ni::UpdateComponent
{
public:
	SlingshotUpdateComponent(ni::ComponentLocator& component_locator);
	void Update() override;

private:
	bool is_dragging_ = false;

	sf::Vector2i mouse_start_position_   = {};
	sf::Vector2i mouse_current_position_ = {};
};

