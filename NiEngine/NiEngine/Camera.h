#pragma once

#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Rect.hpp>

namespace ni {

class Camera
{
private:
	sf::View view_;

public:
	sf::View& GetView()
	{
		return view_;
	}

	sf::Vector2f GetCoordinatesFromPixel(sf::RenderTarget& target, const sf::Vector2i& position) const;

	void FitTo(sf::FloatRect bounds);
	void SetPosition(sf::Vector2f position);
	void ApplyTo(sf::RenderTarget& target);	
};

}