#pragma once

#include <string>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include <NiEngine/BitmapStore.h>
#include <NiEngine/GameModeController.h>

namespace ni {

class Engine
{

private:
	sf::RenderWindow window_;

	GameModeController game_mode_controller_;

	BitmapStore bitmap_store_;

public:
	inline static bool exit_game_ = false;

	static sf::Time		time_elapsed;
	static sf::Vector2u window_resolution;

	Engine(std::string window_name, sf::State start_state);

	GameModeController& GetGameModeController();

	void Run();
};

}