#include "AmmoUpdateComponent.h"

#include <box2d.h>
#include <math_functions.h>

#include <iostream>

#include <NiEngine/DynamicBodyPhysicsComponent.h>
#include <NiEngine/Converter.h>
#include <NiEngine/TransformComponent.h>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/UpdateComponent.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/ServiceLocator.h>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>

AmmoUpdateComponent::AmmoUpdateComponent(ni::Id<ni::GameObjectTag> owner_id, ni::ComponentLocator& component_locator) : UpdateComponent(component_locator)
{
	owner_id_ = owner_id;

	ni::ServiceLocator::Instance().GetEventDispatcher().OnMouseButtonPressed([this](const sf::Event::MouseButtonPressed& event) {
		if (active_ && !ability_used_ && event.button == sf::Mouse::Button::Left)
		{ 
			ActivateAbility();

			ability_used_ = true;
		}
	});
}

void AmmoUpdateComponent::Launch(b2Vec2 direction, float impulse_ratio)
{
	auto ammo_physics = static_cast<ni::DynamicBodyPhysicsComponent*>(component_locator_.GetPhysicsComponent(owner_id_));
	ni::TransformComponent* ammo_transform = component_locator_.GetTransformComponent(owner_id_);

	if (!ammo_physics || !ammo_transform)
	{
#ifdef _DEBUG
		std::cout << "\nAmmo physics and transform not found!!!";
#endif // _DEBUG
		return;
	}
	b2Body_SetTransform(ammo_physics->GetBodyId(), ni::Converter::PixelsToMeters(ammo_transform->GetTransformable().getPosition()), b2Rot_identity);

	direction *= -1;
	b2Vec2 velocity = direction * (impulse_ratio * max_speed_);

	ammo_physics->Activate();
	b2Body_SetLinearVelocity(ammo_physics->GetBodyId(), velocity);

	active_ = true;
}

void AmmoUpdateComponent::Update()
{
}

void AmmoUpdateComponent::ActivateAbility()
{
	auto ammo_physics = static_cast<ni::DynamicBodyPhysicsComponent*>(component_locator_.GetPhysicsComponent(owner_id_));

	b2Vec2 velocity = b2Body_GetLinearVelocity(ammo_physics->GetBodyId());
	velocity *= 3;
	b2Body_SetLinearVelocity(ammo_physics->GetBodyId(), velocity);
}
