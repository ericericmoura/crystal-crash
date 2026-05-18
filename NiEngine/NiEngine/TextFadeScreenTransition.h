#pragma once

#include <string>
#include <memory>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "ScreenTransition.h"
#include "BitmapStore.h"

namespace ni {

class TextFadeScreenTransition : public ScreenTransition
{
public:
	TextFadeScreenTransition() = default;
	TextFadeScreenTransition(float delay_in_seconds, const std::string& text_string, const std::string& font_path, int font_size, sf::Color text_color, sf::Color background_color, sf::Vector2f camera_size);

	void Init(float delay_in_seconds, const std::string& text_string, const std::string& font_path, int font_size, sf::Color text_color, sf::Color background_color, sf::Vector2f camera_size);
	
	void SetDelayInSeconds(float delay_in_seconds)    { delay_in_seconds_ = delay_in_seconds;  }
	void SetTextString    (const std::string& string) { main_text_->setString(string); }

	void StopHalfway() { stop_halfway_ = true; };

	void Update() override;
	void Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store) override;

private:
	std::unique_ptr<sf::Font> main_font_;
	std::unique_ptr<sf::Text> main_text_;
	sf::RectangleShape background_;

	bool stop_halfway_ = false;

	void LerpTextOpacity(float time_elapsed);
};

}