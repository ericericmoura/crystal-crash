#pragma once

#include <memory>
#include <unordered_map>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <NiEngine/GameModeTag.h>
#include <NiEngine/BitmapStore.h>
#include <NiEngine/GameMode.h>
#include <NiEngine/Id.h>

namespace ni {

class GameModeController
{
private:
	std::unordered_map<Id<GameModeTag>, std::unique_ptr<GameMode>> game_modes_;
	
	Id<GameModeTag> current_game_mode;
	Id<GameModeTag> next_id;

public:
	GameModeController();

	Id<GameModeTag> Register(std::unique_ptr<GameMode> mode);
	
	void SwitchTo(Id<GameModeTag> id);
	void Remove(Id<GameModeTag> id);

	GameMode& Get(Id<GameModeTag> id);
	GameMode& GetCurrent();
	
	virtual void PhysicsUpdate(float delta);
	virtual void Update();
	virtual void Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store);
};

}