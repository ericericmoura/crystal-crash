#pragma once

#include <id.h>
#include <types.h>

namespace ni {

class PhysicsEngine
{
private:
	inline static const float kTimeStep     = 1.0f / 60.0f;
	inline static const int   kSubStepCount = 4;

	b2WorldId world_id_;

public:
	PhysicsEngine() = default;
	PhysicsEngine(const b2WorldDef& world_definition);

	void CreateWorld(const b2WorldDef& world_definition);
	
	b2WorldId GetWorldId() const
	{
		return world_id_;
	}

	void PhysicsUpdate();
};

}