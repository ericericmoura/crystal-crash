#include "ExitDoorUpdateComponent.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <NiEngine/ComponentLocator.h>
#include <NiEngine/GameMode.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>

#include "ObstacleUpdateComponent.h"
#include "PlatformerGameMode.h"

ExitDoorUpdateComponent::ExitDoorUpdateComponent(ni::GameMode& mode, ni::ComponentLocator& component_locator, ni::Id<ni::GameObjectTag> id, sf::Vector2f collision_box_size) :
	ObstacleUpdateComponent(component_locator, id, collision_box_size)
{
	on_player_enter_.Subscribe([&mode]() {
		auto& platformer_mode = static_cast<PlatformerGameMode&>(mode);
		
		platformer_mode.PrepareToLoadNextLevel();
	});
}

void ExitDoorUpdateComponent::CollideTop(sf::FloatRect collision_box, ni::Id<ni::GameObjectTag> object_id)
{
	auto player_id = component_locator_.GetIdByTag(PlatformerGameMode::kPlayerTag);

	if (player_id == object_id)
	{
		on_player_enter_.Notify();
	}
}

void ExitDoorUpdateComponent::CollideBottom(sf::FloatRect collision_box, ni::Id<ni::GameObjectTag> object_id)
{
	auto player_id = component_locator_.GetIdByTag(PlatformerGameMode::kPlayerTag);

	if (player_id == object_id)
	{
		on_player_enter_.Notify();
	}
}

void ExitDoorUpdateComponent::CollideFront(sf::FloatRect collision_box, ni::Id<ni::GameObjectTag> object_id)
{
	auto player_id = component_locator_.GetIdByTag(PlatformerGameMode::kPlayerTag);

	if (player_id == object_id)
	{
		on_player_enter_.Notify();
	}
}
