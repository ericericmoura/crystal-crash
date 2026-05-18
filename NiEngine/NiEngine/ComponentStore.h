#pragma once

#include <id.h>

#include <unordered_map>
#include <memory>
#include <utility>
#include <vector>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <NiEngine/Id.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/GraphicsComponent.h>
#include <NiEngine/UpdateComponent.h>
#include <NiEngine/PhysicsComponent.h>
#include <NiEngine/BitmapStore.h>
#include <NiEngine/TransformComponent.h>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/AnimatedGraphicsComponent.h>
#include "Tilemap.h"
#include <string>

namespace ni {

class ComponentStore : public ComponentLocator
{
private:
	std::unordered_map<Id<GameObjectTag>, std::unique_ptr<TransformComponent>>             transform_components_ = {};
	std::unordered_map<Id<GameObjectTag>, std::unique_ptr<PhysicsComponent>>               physics_components_   = {};
	std::unordered_map<Id<GameObjectTag>, std::vector<std::unique_ptr<GraphicsComponent>>> graphics_components_  = {};
	std::unordered_map<Id<GameObjectTag>, std::unique_ptr<UpdateComponent>>                update_components_     = {};

public:
	// Component Attaching/Removing	
	void AttachPhysicsComponent(Id<GameObjectTag> target, std::unique_ptr<PhysicsComponent> component)
	{		
		physics_components_.emplace(target, std::move(component));
	}

	void AttachUpdateComponent(Id<GameObjectTag> target, std::unique_ptr<UpdateComponent> component)
	{
		update_components_.emplace(target, std::move(component));
	}

	void AttachGraphicsComponent(Id<GameObjectTag> target, std::unique_ptr<GraphicsComponent> component)
	{
		graphics_components_[target].emplace_back(std::move(component));
	}

	void AttachTransformComponent(Id<GameObjectTag> target, TransformComponent& component)
	{
		transform_components_.emplace(target, std::make_unique<TransformComponent>(component));
	}

	void RemovePhysicsComponent(Id<GameObjectTag> target)
	{
		physics_components_.erase(target);
	}

	void RemoveGraphicsComponent(Id<GameObjectTag> target)
	{
		graphics_components_.erase(target);
	}

	void RemoveTransformComponent(Id<GameObjectTag> target)
	{
		transform_components_.erase(target);
	}

	void Clear();

	// Inherited via ComponentLocator
	std::vector<GraphicsComponent*> GetGraphicsComponents(Id<GameObjectTag> id)				override;
	TransformComponent*             GetTransformComponent(Id<GameObjectTag> id)				override;
	PhysicsComponent*	            GetPhysicsComponent(Id<GameObjectTag> id)               override;
	AnimatedGraphicsComponent*      GetFirstAnimatedGraphicsComponent(Id<GameObjectTag> id) override;
	UpdateComponent* GetUpdateComponent(Id<GameObjectTag> id) override;

	std::vector<ni::Id<ni::GameObjectTag>> GetIdsByTag(std::string tag) override;
	Id<GameObjectTag> GetIdByTag(std::string tag) override;

	void RegisterTagForId(Id<GameObjectTag> target, std::string tag);

	void PhysicsUpdate(b2WorldId world_id, const Tilemap& current_tilemap, float delta);
	void Update();
	void Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store);


};

}