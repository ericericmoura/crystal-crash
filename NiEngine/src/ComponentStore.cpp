#include <NiEngine/ComponentStore.h>

#include <id.h>
#include <box2d.h>
#include <types.h>

#include <vector>
#include <string>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <NiEngine/BitmapStore.h>
#include <NiEngine/Id.h>
#include <NiEngine/TransformComponent.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/PhysicsComponent.h>
#include <NiEngine/GraphicsComponent.h>
#include <NiEngine/AnimatedGraphicsComponent.h>
#include <NiEngine/Tilemap.h>
#include <NiEngine/UpdateComponent.h>
#include <NiEngine/GameMode.h>

ni::TransformComponent* ni::ComponentStore::GetTransformComponent(Id<ni::GameObjectTag> id)
{
	auto it = transform_components_.find(id);

	if (it == transform_components_.end())
	{
		return nullptr;
	}
	return it->second.get();
}

ni::PhysicsComponent* ni::ComponentStore::GetPhysicsComponent(Id<ni::GameObjectTag> id)
{
	auto it = physics_components_.find(id);

	if (it == physics_components_.end())
	{
		return nullptr;
	}
	return it->second.get();
}

ni::AnimatedGraphicsComponent* ni::ComponentStore::GetFirstAnimatedGraphicsComponent(Id<ni::GameObjectTag> id)
{
	std::vector<ni::GraphicsComponent*> components = GetGraphicsComponents(id);
	for (auto& component : components)
	{
		auto animated_graphics = dynamic_cast<AnimatedGraphicsComponent*>(component);

		if (animated_graphics)
		{
			return animated_graphics;
		}
	}
	return nullptr;
}

void ni::ComponentStore::RegisterTagForId(Id<GameObjectTag> target, std::string tag)
{
	id_tag_map_[tag].push_back(target);
}

void ni::ComponentStore::PhysicsUpdate(b2WorldId world_id, const Tilemap& current_tilemap, float delta)
{
	if (physics_components_.empty())
	{
		return;
	}
	
	HandleBox2dEvents(world_id);

	for (auto& id : ids_marked_for_deletion_)
	{
		if (physics_components_.contains(id))
		{
			physics_components_.erase(id);
		}
	}
}

void ni::ComponentStore::Update()
{
	if (update_components_.empty())
	{
		return;
	}
	for (auto& [id, component] : update_components_)
	{
		component->Update();
	}

	for (auto& id : ids_marked_for_deletion_)
	{
		if (update_components_.contains(id))
		{
			update_components_.erase(id);
		}
	}
}

void ni::ComponentStore::Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store)
{
	if (graphics_components_.empty())
	{
		return;
	}
	for (auto& [id, components] : graphics_components_)
	{
		sf::RenderStates local_state = states;

		TransformComponent* transform = GetTransformComponent(id);
		if (transform)
		{
			local_state.transform *= transform->GetTransformable().getTransform();			
		}

		for (auto& component : components)
		{
			component->Render(target, local_state, store);
		}		
	}

	for (auto& id : ids_marked_for_deletion_)
	{
		if (graphics_components_.contains(id))
		{
			graphics_components_.erase(id);
		}
		if (transform_components_.contains(id))
		{
			transform_components_.erase(id);
		}
	}
}

ni::UpdateComponent* ni::ComponentStore::GetUpdateComponent(Id<GameObjectTag> id)
{
	auto it = update_components_.find(id);

	if (it == update_components_.end())
	{
		return nullptr;
	}
	return it->second.get();
}

std::vector<ni::Id<ni::GameObjectTag>> ni::ComponentStore::GetIdsByTag(std::string tag)
{
	auto it = id_tag_map_.find(tag);
	if (it == id_tag_map_.end())
	{
		return {};
	}
	return it->second;
}

ni::Id<ni::GameObjectTag> ni::ComponentStore::GetIdByTag(std::string tag)
{
	return GetIdsByTag(tag).front();
}

void ni::ComponentStore::HandleBox2dEvents(b2WorldId world_id)
{
	b2BodyEvents events = b2World_GetBodyEvents(world_id);
	for (int i = 0; i < events.moveCount; ++i)
	{
		const b2BodyMoveEvent* event = events.moveEvents + i;
		auto id = static_cast<ni::Id<ni::GameObjectTag>*>(event->userData);

		TransformComponent* transform_component = GetTransformComponent(*id);
		PhysicsComponent* physics = GetPhysicsComponent(*id);

		if (!transform_component || !physics)
		{
			continue;
		}
		physics->PhysicsUpdate(*transform_component, world_id, event->transform);
	}
}

void ni::ComponentStore::Clear()
{
	transform_components_.clear();
	update_components_   .clear();
	graphics_components_ .clear();
	physics_components_  .clear();
	id_tag_map_.clear();
}

std::vector<ni::GraphicsComponent*> ni::ComponentStore::GetGraphicsComponents(Id<ni::GameObjectTag> id)
{
	std::vector<ni::GraphicsComponent*> result = {};

	auto it = graphics_components_.find(id);
	if (it == graphics_components_.end())
	{
		return result;
	}

	result.reserve(graphics_components_.at(id).size());
	for (auto& comp : graphics_components_.at(id))
	{
		result.push_back(comp.get());
	}

	return result;
}

void ni::ComponentStore::SpawnComponents(ni::GameMode& mode)
{
	if (update_components_.empty())
	{
		return;
	}
	for (auto& [id, component] : update_components_)
	{
		component->SpawnComponents(mode);
	}
}

void ni::ComponentStore::MarkIdForDeletion(ni::Id<GameObjectTag> id)
{
	ids_marked_for_deletion_.emplace(id);
}