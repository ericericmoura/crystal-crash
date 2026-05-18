#include <NiEngine/ComponentStore.h>

#include <id.h>

#include <vector>
#include <iostream>

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
#include <string>
#include <NiEngine/UpdateComponent.h>

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
	for (auto& [id, component] : physics_components_)
	{
		TransformComponent* transform = GetTransformComponent(id);

		if (!transform)
		{
			continue;
		}
		component->PhysicsUpdate(*transform, current_tilemap, delta);
		component->PhysicsUpdate(*transform, world_id);
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
