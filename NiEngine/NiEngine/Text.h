#pragma once

#include <string>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Color.hpp>

#include "HUDComponent.h"
#include "BitmapStore.h"

namespace ni {

class Text : public HUDComponent
{
public:
	Text(const std::string& font_filepath, const std::string& string, sf::Color color, int size, bool smooth = false);
	
	void SetTextOutline(int size, sf::Color color);
	void SetTextString(const std::string& string) { text_.setString(string); }
	void Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store) override;

private:
	sf::Text text_;
	sf::Text subtext_;

	sf::Font font_;
};

}