#include "SlingshotUpdateComponent.h"

#include <math_functions.h>
#include <box2d.h>

#include <iostream>
#include <cstdint>

#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Vector2.hpp>
#include <NiEngine/UpdateComponent.h>
#include <NiEngine/ServiceLocator.h>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/TransformComponent.h>
#include <NiEngine/GameMode.h>
#include <NiEngine/Converter.h>
#include <NiEngine/DynamicBodyPhysicsComponent.h>

#include "PlatformerGameMode.h"

SlingshotUpdateComponent::SlingshotUpdateComponent(ni::ComponentLocator& component_locator, sf::Vector2f start_position, sf::Vector2i sprite_size) : ni::UpdateComponent(component_locator)
{
	initial_ammo_position_    = start_position;
	initial_ammo_position_.x += sprite_size.x / 2.0f;

	ni::ServiceLocator::Instance().GetEventDispatcher().OnMouseButtonPressed([this](const sf::Event::MouseButtonPressed& event) {
		if (event.button == sf::Mouse::Button::Left)
		{
			mouse_start_position_ = ni::GameMode::GetMousePositionInWorldCoordinates();

			if (mouse_start_position_.x - kMinMouseDistance > initial_ammo_position_.x ||
				(mouse_start_position_ - initial_ammo_position_).length() > kMinMouseDistance)
			{
				return;
			}

			is_dragging_ = true;
		}
	});

	ni::ServiceLocator::Instance().GetEventDispatcher().OnMouseButtonReleased([this](const sf::Event::MouseButtonReleased& event) {
		if (event.button == sf::Mouse::Button::Left)
		{
			is_dragging_ = false;

			auto ammo_physics = static_cast<ni::DynamicBodyPhysicsComponent*>(component_locator_.GetPhysicsComponent(current_ammo_id_));
			ni::TransformComponent* ammo_transform = component_locator_.GetTransformComponent(current_ammo_id_);

			if (!ammo_physics || !ammo_transform)
			{
#ifdef _DEBUG
				std::cout << "\nAmmo physics and transform not found!!!";
#endif // _DEBUG
				return;
			}
			b2Body_SetTransform(ammo_physics->GetBodyId(), ni::Converter::PixelsToMeters(ammo_transform->GetTransformable().getPosition()), b2Rot_identity);

			launch_direction_ *= -1;
			b2Vec2 velocity = launch_direction_ * 20;

			ammo_physics->Activate();
			b2Body_SetLinearVelocity(ammo_physics->GetBodyId(), velocity);
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

	mouse_current_position_ = ni::GameMode::GetMousePositionInWorldCoordinates();

	sf::Vector2f dir = mouse_current_position_ - mouse_start_position_;
	if (dir.x == 0 && dir.y == 0)
	{
		return;
	}

	float distance = std::min(dir.length(), kMaxDragDistance);

	launch_force_     = distance;
	dir = dir.normalized();
	launch_direction_ = { dir.x, dir.y };

	sf::Vector2f ammo_position = initial_ammo_position_;
	ammo_position += dir * distance;

	ammo_transform->GetTransformable().setPosition(ammo_position);
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
