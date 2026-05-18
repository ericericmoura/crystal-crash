#include <NiEngine/Engine.h>

#include <string>

#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowEnums.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include <NiEngine/GameModeController.h>
#include <NiEngine/ServiceLocator.h>

sf::Time     ni::Engine::time_elapsed		= sf::Time();
sf::Vector2u ni::Engine::window_resolution  = {};

ni::Engine::Engine(std::string window_name, sf::State start_state)
{
	window_resolution = sf::VideoMode::getDesktopMode().size;

	window_.create(sf::VideoMode::getDesktopMode(), window_name, start_state);
	window_.setKeyRepeatEnabled(false);
	window_.setFramerateLimit(60);
}

ni::GameModeController& ni::Engine::GetGameModeController()
{
	return game_mode_controller_;
}

void ni::Engine::Run()
{
	sf::Clock delta_clock;
	while (window_.isOpen())
	{
		if (Engine::exit_game_)
		{
			window_.close();
			break;
		}

		while (const std::optional event = window_.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window_.close();
			}
			event->visit(ServiceLocator::Instance().GetEventDispatcher());
		}

		sf::Time delta_time = delta_clock.restart();
		time_elapsed += delta_time;

		game_mode_controller_.Update();
		game_mode_controller_.PhysicsUpdate(delta_time.asSeconds());

		ServiceLocator::Instance().GetSoundEngine().FlushSoundQueue();

		window_.clear(sf::Color::Black);

		game_mode_controller_.Render(window_, sf::RenderStates::Default, bitmap_store_);

		window_.display();

	}
}
