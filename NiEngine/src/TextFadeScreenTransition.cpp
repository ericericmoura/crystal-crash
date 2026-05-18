#include <NiEngine/TextFadeScreenTransition.h>

#include <string>
#include <cmath>
#include <memory>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <NiEngine/ScreenTransition.h>
#include <NiEngine/Engine.h>
#include <NiEngine/BitmapStore.h>

ni::TextFadeScreenTransition::TextFadeScreenTransition(
	float delay_in_seconds, 
	const std::string& text_string,
	const std::string& font_path,
	int font_size,
	sf::Color text_color, 
	sf::Color background_color, 
	sf::Vector2f camera_size) : ScreenTransition(delay_in_seconds)
{	
	Init(delay_in_seconds, text_string, font_path, font_size, text_color, background_color, camera_size);
}

void ni::TextFadeScreenTransition::Init(float delay_in_seconds, const std::string& text_string, const std::string& font_path, int font_size, sf::Color text_color, sf::Color background_color, sf::Vector2f camera_size)
{
	main_font_ = std::make_unique<sf::Font>(font_path);
	main_font_->setSmooth(false);
	main_text_ = std::make_unique<sf::Text>(*main_font_, text_string, font_size);

	delay_in_seconds_ = delay_in_seconds;
	main_text_->setFillColor(text_color);

	sf::Vector2f main_text_position = camera_size;
	main_text_position -= camera_size / 2.0f;	

	main_text_position.x -= main_text_->getGlobalBounds().size.x / 2.0f;
	main_text_->setPosition(main_text_position);
	background_.setPosition({ 0, 0 });
	background_.setSize(camera_size);
	background_.setFillColor(background_color);
}

void ni::TextFadeScreenTransition::Update()
{
	if (!playing_ && !playing_reversed_)
	{
		return;
	}
	if (playing_reversed_ && stop_halfway_)
	{
		return;
	}
	float time_elapsed = (Engine::time_elapsed - time_since_start_).asSeconds();
	if (time_elapsed < delay_in_seconds_ / 2.0f)
	{
		LerpTextOpacity(time_elapsed);
		return;
	}
	if (!playing_reversed_)
	{
		LerpTextOpacity(1);
		playing_reversed_ = true;
		time_since_start_ = Engine::time_elapsed;
		on_transition_covered_screen_.Notify();
		return;
	}
	on_transition_finished_.Notify();
	playing_ = false;
	playing_reversed_ = false;

	background_.setFillColor(sf::Color::Transparent);
	main_text_->setFillColor(sf::Color::Transparent);
}

void ni::TextFadeScreenTransition::Render(sf::RenderTarget & target, sf::RenderStates states, BitmapStore & store)
{
	if (!playing_)
	{
		return;
	}
	target.draw(background_, states);
	if (main_text_)
	{
		target.draw(*main_text_ , states);
	}
}

void ni::TextFadeScreenTransition::LerpTextOpacity(float time_elapsed)
{
	sf::Color text_color = main_text_->getFillColor();

	text_color.a = playing_reversed_ ? std::lerp(255, 0  , time_elapsed / (delay_in_seconds_ / 2.0f)) :
		                               std::lerp(0  , 255, time_elapsed / (delay_in_seconds_ / 2.0f));

	main_text_->setFillColor(text_color);
}

