#include <NiEngine/GameMode.h>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Mouse.hpp>
#include <NiEngine/BitmapStore.h>
#include <NiEngine/GameModeController.h>
#include <NiEngine/PhysicsEngine.h>

ni::GameMode::GameMode()
{
	transitions_camera_.GetView().setViewport({ {0, 0}, {1, 1} });
}

void ni::GameMode::LoadLevel(int index, bool enable_tilemap_collisions)
{
	level_.LoadLevelByIndex(*this, index);
	if (enable_tilemap_collisions)
	{
		level_.EnableTilemapCollisions(physics_engine_.GetWorldId());
	}
}

void ni::GameMode::LoadNextLevel(bool enable_tilemap_collisions)
{
	level_.LoadNextLevel(*this);
	if (enable_tilemap_collisions)
	{
		level_.EnableTilemapCollisions(physics_engine_.GetWorldId());
	}
}

void ni::GameMode::PhysicsUpdate(float delta)
{
	if (paused_)
	{
		return;
	}
	if (box2d_enabled)
	{
		physics_engine_.PhysicsUpdate();
	}
	component_store_.PhysicsUpdate(physics_engine_.GetWorldId(), level_.GetCurrentTilemap(), delta);
}

void ni::GameMode::Update(GameModeController& controller)
{
	if (paused_)
	{
		return;
	}
	component_store_.Update();
	if (current_transition_)
	{
		current_transition_->Update();
	}
	engine_title_transition_.Update();
}

void ni::GameMode::Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store)
{	
	mouse_position_in_world_coodinates_ = world_camera_.GetCoordinatesFromPixel(target, sf::Mouse::getPosition());
}

void ni::GameMode::SpawnComponents()
{
	component_store_.SpawnComponents(*this);
}