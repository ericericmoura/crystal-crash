#include <NiEngine/WipeScreenTransition.h>

#include <cmath>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <NiEngine/ScreenTransition.h>
#include <NiEngine/Engine.h>
#include <NiEngine/BitmapStore.h>
#include <NiEngine/MathUtility.h>

ni::WipeScreenTransition::WipeScreenTransition(float delay_in_seconds, sf::Vector2f camera_size, bool vertical, sf::Color color) : ScreenTransition(delay_in_seconds),
	upper_rect_(camera_size),
	lower_rect_(camera_size)
{
	vertical_ = vertical;
	upper_rect_.setFillColor(color);
	lower_rect_.setFillColor(color);

	upper_rect_.setPosition(-upper_rect_.getSize());
	lower_rect_.setPosition(upper_rect_.getSize());
}

void ni::WipeScreenTransition::MoveRectByAxis(bool y_axis, sf::RectangleShape& rect, int sign, float time_elapsed, bool reversed)
{
	sf::Vector2f position = rect.getPosition();
	float rect_start_y_pos = 0;
	float rect_end_y_pos   = 0;

	float rect_start_x_pos = 0;
	float rect_end_x_pos   = 0;

	position.x = 0;
	position.y = 0;

	float eased_t = MathUtility::EaseInOut(time_elapsed / delay_in_seconds_);
	if (y_axis)
	{
		float rect_start_y_pos = rect.getSize().y * sign;
		float rect_end_y_pos   = rect.getSize().y / 2.0f * sign + (1 * -sign);

		position.y = reversed ? std::lerp(rect_end_y_pos, rect_start_y_pos, eased_t) :
					            std::lerp(rect_start_y_pos, rect_end_y_pos, eased_t);
	}
	else
	{
		float rect_start_x_pos = rect.getSize().x * sign;
		float rect_end_x_pos = rect.getSize().x / 2.0f * sign + (5 * -sign);

		position.x = reversed ? std::lerp(rect_end_x_pos, rect_start_x_pos, eased_t) :
			                    std::lerp(rect_start_x_pos, rect_end_x_pos, eased_t);
	}
	rect.setPosition(position);
}

void ni::WipeScreenTransition::Prepare()
{
	if (playing_reversed_)
	{
		MoveRectByAxis(vertical_, upper_rect_, -1, 1, false);
		MoveRectByAxis(vertical_, lower_rect_,  1, 1, false);
	}
}

void ni::WipeScreenTransition::PlayTransition(float time_elapsed)
{
	MoveRectByAxis(vertical_, upper_rect_, -1, time_elapsed, playing_reversed_);
	MoveRectByAxis(vertical_, lower_rect_,  1, time_elapsed, playing_reversed_);
}

void ni::WipeScreenTransition::Update()
{
	if (!playing_ && !playing_reversed_)
	{
		upper_rect_.setPosition({2000, 2000});
		lower_rect_.setPosition({2000, 2000});
		return;
	}
	float time_elapsed = (Engine::time_elapsed - time_since_start_).asSeconds();
	if (time_elapsed < delay_in_seconds_)
	{
		PlayTransition(time_elapsed);
		return;
	}
	if (!playing_reversed_)
	{
		PlayTransition(1);
		playing_reversed_ = true;
		time_since_start_ = Engine::time_elapsed;

		on_transition_covered_screen_.Notify();
		return;
	}
	on_transition_finished_.Notify();
	playing_ = false;
	playing_reversed_ = false;
}

void ni::WipeScreenTransition::Render(sf::RenderTarget & target, sf::RenderStates states, BitmapStore & store)
{
	target.draw(upper_rect_, states);
	target.draw(lower_rect_, states);
}

