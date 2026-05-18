#include <NiEngine/PhysicsEngine.h>

#include <types.h>
#include <box2d.h>

ni::PhysicsEngine::PhysicsEngine(const b2WorldDef& world_definition)
{
	CreateWorld(world_definition);
}

void ni::PhysicsEngine::CreateWorld(const b2WorldDef& world_definition)
{
	world_id_ = b2CreateWorld(&world_definition);
}

void ni::PhysicsEngine::PhysicsUpdate()
{
	b2World_Step(world_id_, kTimeStep, kSubStepCount);
}
