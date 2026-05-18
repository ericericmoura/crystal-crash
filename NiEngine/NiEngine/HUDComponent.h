#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Rect.hpp>

#include "BitmapStore.h"

namespace ni {

class HUDComponent
{
public:
	sf::FloatRect& GetBounds() { return bounds_;  };

	virtual void Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store) = 0;

protected:
	sf::FloatRect bounds_;
};

}