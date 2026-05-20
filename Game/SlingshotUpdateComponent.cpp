#include "SlingshotUpdateComponent.h"

#include <iostream>
#include <cstdint>

#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Vector2.hpp>
#include <NiEngine/UpdateComponent.h>
#include <NiEngine/ServiceLocator.h>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/TransformComponent.h>

#include "PlatformerGameMode.h"

SlingshotUpdateComponent::SlingshotUpdateComponent(ni::ComponentLocator& component_locator, sf::Vector2f start_position, sf::Vector2i sprite_size) : ni::UpdateComponent(component_locator)
{
	initial_ammo_position_    = start_position;
	initial_ammo_position_.x += sprite_size.x / 2.0f;

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
	if (ammo_queue_.empty())
	{
		if (!loaded_ammo_queue_)
		{
			LoadAmmoQueue();
		}
		else return;
	}
	if (!is_dragging_)
	{
		return;
	}
	if (current_ammo_id_.id_ == UINT32_MAX)
	{
		Reload();
	}

	ni::TransformComponent* ammo_transform = component_locator_.GetTransformComponent(current_ammo_id_);	

	mouse_current_position_ = sf::Mouse::getPosition();

	sf::Vector2f dir	= sf::Vector2f(mouse_current_position_ - mouse_start_position_);
	float distance = dir.length();
	float deg      = 0;
	if (distance != 0)
	{
		deg = dir.angle().asDegrees();
	}


	
	//std::cout << "\nmouse drag angle: "    << deg;
	//std::cout << "\nmouse drag distance: " << distance;
	//std::cout << "\n";
}

void SlingshotUpdateComponent::Reload()
{
	current_ammo_id_ = ammo_queue_.front();
	ammo_queue_.pop();
}

void SlingshotUpdateComponent::LoadAmmoQueue()
{
	auto ammo_list = component_locator_.GetIdsByTag(PlatformerGameMode::kSlingshotAmmoTag);
	for (auto ammo : ammo_list)
	{
		ammo_queue_.push(ammo);
	}
	loaded_ammo_queue_ = true;

	Reload();

	ni::TransformComponent* ammo_transform = component_locator_.GetTransformComponent(current_ammo_id_);
	ammo_transform->GetTransformable().setPosition(initial_ammo_position_);
}
