#pragma once

#include <id.h>
#include <math_functions.h>

#include <NiEngine/Component.h>
#include <NiEngine/Tilemap.h>

namespace ni {

class TransformComponent;

class PhysicsComponent : public Component
{
public:
	virtual void PhysicsUpdate(TransformComponent& transform_component, b2WorldId world_id, b2Transform transform) {};
	virtual void PhysicsUpdate(TransformComponent& transform_component, const Tilemap& current_tilemap, float delta) {};
};

}