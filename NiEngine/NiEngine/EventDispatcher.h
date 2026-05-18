#pragma once

#include <functional>

#include <SFML/Window/Event.hpp>

#include "Subject.h"

namespace ni {

class EventDispatcher
{
private:
	static bool Instantiated;

	Subject<const sf::Event::Closed&>	   on_closed_;
	Subject<const sf::Event::KeyPressed&>  on_key_pressed_;
	Subject<const sf::Event::KeyReleased&> on_key_released_;

public:
	EventDispatcher();	

	int OnClosed  	 (std::function<void(const sf::Event::Closed&)> callback)	   { return on_closed_.Subscribe(callback);		  };
	int OnKeyPressed (std::function<void(const sf::Event::KeyPressed&)> callback)  { return on_key_pressed_.Subscribe(callback);  };
	int OnKeyReleased(std::function<void(const sf::Event::KeyReleased&)> callback) { return on_key_released_.Subscribe(callback); };

	void RemoveKeyPressedEvent (int id) { on_key_pressed_.Remove(id);  };
	void RemoveKeyReleasedEvent(int id) { on_key_released_.Remove(id); };

	void operator()(const sf::Event::Closed& event);
	void operator()(const sf::Event::KeyPressed& event);
	void operator()(const sf::Event::KeyReleased& event);

	template <typename T>
	void operator()(const T&) const {};
};

}