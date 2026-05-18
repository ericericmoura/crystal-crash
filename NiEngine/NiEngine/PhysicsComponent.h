#pragma once

#include <NiEngine/Component.h>
#include <NiEngine/Tilemap.h>
#include <id.h>

namespace ni {

class TransformComponent;

class PhysicsComponent : public Component
{
public:
	virtual void PhysicsUpdate(TransformComponent& transform_component, b2WorldId world_id) {};
	virtual void PhysicsUpdate(TransformComponent& transform_component, const Tilemap& current_tilemap, float delta) {};
};

}