#include "SlingshotUpdateComponent.h"

#include <math_functions.h>

#include <algorithm>
#include <cstdint>
#include <cmath>

#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Vector2.hpp>
#include <NiEngine/UpdateComponent.h>
#include <NiEngine/ServiceLocator.h>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/TransformComponent.h>
#include <NiEngine/GameMode.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/StandardGraphicsComponent.h>

#include "AmmoUpdateComponent.h"
#include "PlatformerGameMode.h"

SlingshotUpdateComponent::SlingshotUpdateComponent(ni::ComponentLocator& component_locator, sf::Vector2f start_position, sf::Vector2i sprite_size, ni::Id<ni::GameObjectTag> chain_id) : ni::UpdateComponent(component_locator)
{
	chain_id_ = chain_id;

	initial_ammo_position_    = start_position;
	initial_ammo_position_.x += sprite_size.x / 2.0f;

	ni::TransformComponent* chain_transform = component_locator_.GetTransformComponent(chain_id_);
	chain_transform->GetTransformable().setPosition(initial_ammo_position_);

	auto chain_graphics = static_cast<ni::StandardGraphicsComponent*>(component_locator_.GetGraphicsComponents(chain_id_).front());
	chain_graphics->SetTextureRepeating(true);

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

			float length        = std::min (drag_distance_, kMaxDragDistance);
			float impulse_ratio = std::sqrt(length / kMaxDragDistance);

			auto ammo_update = static_cast<AmmoUpdateComponent*>(component_locator_.GetUpdateComponent(current_ammo_id_));

			if (ammo_update)
			{
				ammo_update->Launch(drag_direction_, impulse_ratio);

				Reload();
			}
		}
	});
}

void SlingshotUpdateComponent::Update()
{
	if (ammo_queue_.empty() && !loaded_ammo_queue_)
	{
		LoadAmmoQueue();
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

	drag_distance_ = distance;

	dir = dir.normalized();
	drag_direction_ = { dir.x, dir.y };

	ni::TransformComponent* chain_transform = component_locator_.GetTransformComponent(chain_id_);
	chain_transform->GetTransformable().setRotation(dir.angle() - sf::degrees(90));	

	auto chain_graphics = static_cast<ni::StandardGraphicsComponent*>(component_locator_.GetGraphicsComponents(chain_id_).front());
	chain_graphics->SetTextureFrameRect({ {0, 0}, {7, static_cast<int>(drag_distance_) } });

	sf::Vector2f ammo_position = initial_ammo_position_;
	ammo_position += dir * distance;

	ammo_transform->GetTransformable().setPosition(ammo_position);
}

void SlingshotUpdateComponent::Reload()
{
	if (ammo_queue_.empty())
	{
		return;
	}
	current_ammo_id_ = ammo_queue_.front();
	ammo_queue_.pop();

	ni::TransformComponent* ammo_transform = component_locator_.GetTransformComponent(current_ammo_id_);
	ammo_transform->GetTransformable().setPosition(initial_ammo_position_);
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
}
