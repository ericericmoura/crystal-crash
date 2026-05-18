#pragma once

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>

#include "Container.h"
#include "BitmapStore.h"

namespace ni {

class Panel : public Container
{
public:
	Panel(sf::Color background_color, sf::Vector2f gap, sf::Vector2f margin, bool vertical, int max_columns, sf::FloatRect rect);
	Panel(sf::Color background_color, sf::Vector2f gap, sf::Vector2f margin, bool vertical, int max_columns, sf::Vector2f position);

	virtual void Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store) override;

private:
	sf::RectangleShape background_;
};

}