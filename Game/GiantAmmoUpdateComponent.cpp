#include "GiantAmmoUpdateComponent.h"

#include <box2d.h>
#include <collision.h>
#include <id.h>
#include <types.h>

#include <SFML/System/Vector2.hpp>

#include <NiEngine/PolygonBlueprint.h>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/PolygonUtility.h>
#include <NiEngine/DynamicBodyPhysicsComponent.h>

#include "AmmoUpdateComponent.h"

GiantAmmoUpdateComponent::GiantAmmoUpdateComponent(ni::Id<ni::GameObjectTag> owner_id, ni::ComponentLocator& component_locator, float grow_factor, ni::PolygonBlueprint polygon_blueprint, sf::Vector2i tile_size) :
	AmmoUpdateComponent(owner_id, component_locator)
{
	polygon_blueprint_ = polygon_blueprint;
	tile_size_		   = tile_size;
}

void GiantAmmoUpdateComponent::ActivateAbility()
{
	auto ammo_physics = static_cast<ni::DynamicBodyPhysicsComponent*>(component_locator_.GetPhysicsComponent(owner_id_));	
	
	b2ShapeId* shape_array = nullptr;
	b2Body_GetShapes(ammo_physics->GetBodyId(), shape_array, 1);

	if (!shape_array)
	{
		b2DestroyShape(*shape_array, false);
	}

	b2ShapeDef shape_definition = GetAmmoShapeDefinition();

	b2Polygon polygon = ni::PolygonUtility::CreatePolygonFromOffsetPoints(polygon_blueprint_, tile_size_);
	b2CreatePolygonShape(ammo_physics->GetBodyId(), &shape_definition, &polygon);
}
