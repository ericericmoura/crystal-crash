#pragma once

#include <id.h>

#include "PhysicsComponent.h"
#include "TransformComponent.h"

namespace ni {

class DynamicBodyPhysicsComponent : public PhysicsComponent
{
public:
	DynamicBodyPhysicsComponent(b2BodyId body_id, bool start_active = false);
	~DynamicBodyPhysicsComponent();
	
	b2BodyId GetBodyId() const { return body_id_; }

	void Activate();
	void Deactivate();

	void PhysicsUpdate(TransformComponent& transform_component, b2WorldId world_id, b2Transform transform) override;

private:
	b2BodyId body_id_;

	bool active_ = false;
};

}