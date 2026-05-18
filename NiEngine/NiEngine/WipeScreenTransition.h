#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "ScreenTransition.h"
#include "BitmapStore.h"
#include <SFML/System/Vector2.hpp>

namespace ni {

class WipeScreenTransition : public ScreenTransition
{
public:
	WipeScreenTransition(float delay_in_seconds, sf::Vector2f camera_size, bool vertical, sf::Color color = sf::Color::Black);

	void Update() override;
	void Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store) override;

private:
	bool vertical_ = false;

	sf::RectangleShape upper_rect_;
	sf::RectangleShape lower_rect_;
	
	virtual void Prepare() override;

	void PlayTransition(float time_elapsed);
	void MoveRectByAxis(bool y_axis, sf::RectangleShape& rect, int sign, float time_elapsed, bool reversed = false);
};

}