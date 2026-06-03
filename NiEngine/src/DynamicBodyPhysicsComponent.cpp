#include <NiEngine/DynamicBodyPhysicsComponent.h>

#include <id.h>
#include <box2d.h>
#include <math_functions.h>

#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>
#include <NiEngine/TransformComponent.h>
#include <NiEngine/Converter.h>

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

void ni::DynamicBodyPhysicsComponent::PhysicsUpdate(TransformComponent& transform_component, b2WorldId world_id, b2Transform transform)
{
	if (!active_)
	{
		return;
	}
	sf::Angle    rotation = sf::radians(b2Rot_GetAngle(transform.q));
	sf::Vector2f position = ni::Converter::MetersToPixels(transform.p);

	transform_component.GetTransformable().setPosition(position);
	transform_component.GetTransformable().setRotation(rotation);
}
