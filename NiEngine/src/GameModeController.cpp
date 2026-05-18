#include <NiEngine/GameModeController.h>

#include <memory>
#include <utility>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <NiEngine/BitmapStore.h>
#include <NiEngine/GameMode.h>
#include <NiEngine/GameModeTag.h>
#include <NiEngine/Id.h>

ni::GameModeController::GameModeController()
{
	current_game_mode = { 0 };
	next_id = { 0 };
}

ni::Id<ni::GameModeTag> ni::GameModeController::Register(std::unique_ptr<GameMode> mode)
{
	game_modes_.emplace(++next_id, std::move(mode));

	current_game_mode = next_id;

	return next_id;
}

void ni::GameModeController::SwitchTo(Id<ni::GameModeTag> id)
{
	current_game_mode = id;
}

void ni::GameModeController::Remove(Id<ni::GameModeTag> id)
{
	game_modes_.erase(id);
}

ni::GameMode& ni::GameModeController::Get(Id<ni::GameModeTag> id)
{
	return *game_modes_.at(id).get();
}

ni::GameMode& ni::GameModeController::GetCurrent()
{
	return *game_modes_.at(current_game_mode).get();
}

void ni::GameModeController::PhysicsUpdate(float delta)
{
	if (current_game_mode.id_ == 0)
	{
		return;
	}
	game_modes_.at(current_game_mode)->PhysicsUpdate(delta);
}

void ni::GameModeController::Update()
{	
	if (current_game_mode.id_ == 0)
	{
		return;
	}
	game_modes_.at(current_game_mode)->Update(*this);
}

void ni::GameModeController::Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store)
{
	if (current_game_mode.id_ == 0)
	{
		return;
	}
	game_modes_.at(current_game_mode)->Render(target, states, store);
}
