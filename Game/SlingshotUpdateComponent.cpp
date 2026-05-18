#include "SlingshotUpdateComponent.h"

#include <iostream>

#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Vector2.hpp>
#include <NiEngine/UpdateComponent.h>
#include <NiEngine/ServiceLocator.h>
#include <NiEngine/ComponentLocator.h>

SlingshotUpdateComponent::SlingshotUpdateComponent(ni::ComponentLocator& component_locator) : ni::UpdateComponent(component_locator)
{
	ni::ServiceLocator::Instance().GetEventDispatcher().OnMouseButtonPressed([this](const sf::Event::MouseButtonPressed& event) {
		if (event.button == sf::Mouse::Button::Left)
		{
			is_dragging_ = true;
			mouse_start_position_ = sf::Mouse::getPosition();
		}
	});

	ni::ServiceLocator::Instance().GetEventDispatcher().OnMouseButtonReleased([this](const sf::Event::MouseButtonReleased& event) {
		if (event.button == sf::Mouse::Button::Left)
		{
			is_dragging_ = false;
		}
	});
}

void SlingshotUpdateComponent::Update()
{
	if (!is_dragging_ || (mouse_start_position_.x == 0 && mouse_start_position_.y == 0))
	{
		return;
	}

	mouse_current_position_ = sf::Mouse::getPosition();

	sf::Vector2f dir      = sf::Vector2f(mouse_current_position_ - mouse_start_position_);

	
	float distance = dir.length();
	float deg      = 0;
	if (distance != 0)
	{
		deg = dir.angle().asDegrees();
	}
	
	std::cout << "\nmouse drag angle: "    << deg;
	std::cout << "\nmouse drag distance: " << distance;
	std::cout << "\n";
}
