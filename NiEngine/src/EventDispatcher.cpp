#include <NiEngine/EventDispatcher.h>

#include <cassert>

#include <SFML/Window/Event.hpp>

bool ni::EventDispatcher::Instantiated = false;

ni::EventDispatcher::EventDispatcher()
{
	assert(!Instantiated);
	Instantiated = true;
}

void ni::EventDispatcher::operator()(const sf::Event::Closed& event)
{
	on_closed_.Notify(event);
}

void ni::EventDispatcher::operator()(const sf::Event::KeyPressed& event)
{
	on_key_pressed_.Notify(event);
}

void ni::EventDispatcher::operator()(const sf::Event::KeyReleased& event)
{
	on_key_released_.Notify(event);
}
