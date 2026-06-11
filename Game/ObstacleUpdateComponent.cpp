#include "ObstacleUpdateComponent.h"

#include <box2d.h>
#include <math_functions.h>

#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/UpdateComponent.h>
#include <NiEngine/DynamicBodyPhysicsComponent.h>
#include <NiEngine/Converter.h>
#include <NiEngine/TransformComponent.h>

ObstacleUpdateComponent::ObstacleUpdateComponent(ni::Id<ni::GameObjectTag> owner_id, ni::ComponentLocator& component_locator_) :
	UpdateComponent(component_locator_)
{
	owner_id_ = owner_id;
}

void ObstacleUpdateComponent::Init()
{
	auto physics = static_cast<ni::DynamicBodyPhysicsComponent*>(component_locator_.GetPhysicsComponent(owner_id_));
	ni::TransformComponent* transform = component_locator_.GetTransformComponent(owner_id_);
	
	b2Body_SetTransform(physics->GetBodyId(), ni::Converter::PixelsToMeters(transform->GetTransformable().getPosition()), b2Rot_identity);
	physics->Activate();
}

void ObstacleUpdateComponent::Update()
{
}

void ObstacleUpdateComponent::Damage(float speed)
{
	health_ -= speed;

	if (health_ <= 0)
	{
		component_locator_.MarkIdForDeletion(owner_id_);
	}
}
