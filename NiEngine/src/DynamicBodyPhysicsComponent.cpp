#include <NiEngine/DynamicBodyPhysicsComponent.h>

#include <id.h>
#include <box2d.h>
#include <math_functions.h>

#include <SFML/System/Vector2.hpp>
#include <NiEngine/Converter.h>
#include <NiEngine/TransformComponent.h>

ni::DynamicBodyPhysicsComponent::DynamicBodyPhysicsComponent(b2BodyId body_id, bool start_active)
{
	body_id_ = body_id;
	if (!start_active)
	{
		Deactivate();
	}
}

void ni::DynamicBodyPhysicsComponent::Activate()
{
	active_ = true;
	b2Body_Enable(body_id_);
}

void ni::DynamicBodyPhysicsComponent::Deactivate()
{
	active_ = false;
	b2Body_Disable(body_id_);
}

void ni::DynamicBodyPhysicsComponent::PhysicsUpdate(TransformComponent& transform_component, b2WorldId world_id)
{
	if (!active_)
	{
		return;
	}
	sf::Vector2f body_position = Converter::MetersToPixels(b2Body_GetPosition(body_id_));
	float body_rotation        = b2Rot_GetAngle(b2Body_GetRotation(body_id_));

	transform_component.GetTransformable().setPosition(body_position);
	transform_component.GetTransformable().setRotation(sf::radians(body_rotation));
}
