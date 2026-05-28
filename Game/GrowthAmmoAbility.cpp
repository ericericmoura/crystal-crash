#include "GrowthAmmoAbility.h"

#include <box2d.h>
#include <collision.h>
#include <id.h>
#include <types.h>

#include <SFML/System/Vector2.hpp>
#include <NiEngine/PolygonBlueprint.h>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/DynamicBodyPhysicsComponent.h>
#include <NiEngine/PolygonUtility.h>
#include <NiEngine/TransformComponent.h>

#include "AmmoAbility.h"

GrowthAmmoAbility::GrowthAmmoAbility(ni::PolygonBlueprint polygon_blueprint, sf::Vector2i tile_size, float growth_multiplier, b2ShapeDef shape_definition, bool propagate) 
	: AmmoAbility(propagate)
{
	shape_definition_  = shape_definition;
	polygon_blueprint_ = polygon_blueprint;
	tile_size_		   = tile_size;
	growth_multiplier_ = growth_multiplier;
}

void GrowthAmmoAbility::Activate(ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> ammo_id)
{
	auto ammo_physics = static_cast<ni::DynamicBodyPhysicsComponent*>(component_locator.GetPhysicsComponent(ammo_id));

	b2ShapeId shape_array[1];
	b2Body_GetShapes(ammo_physics->GetBodyId(), shape_array, 1);

	if (shape_array)
	{
		b2DestroyShape(*shape_array, false);
	}

	b2Polygon polygon = ni::PolygonUtility::CreatePolygonFromOffsetPoints(polygon_blueprint_, tile_size_, growth_multiplier_);
	b2CreatePolygonShape(ammo_physics->GetBodyId(), &shape_definition_, &polygon);

	b2Body_ApplyMassFromShapes(ammo_physics->GetBodyId());

	ni::TransformComponent* ammo_transform = component_locator.GetTransformComponent(ammo_id);
	ammo_transform->GetTransformable().setScale({ growth_multiplier_, growth_multiplier_ });	
}
