#include <NiEngine/Text.h>

#include <string>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Color.hpp>
#include <NiEngine/BitmapStore.h>

ni::Text::Text(const std::string& font_filepath, const std::string& string, sf::Color color, int size, bool smooth) :
	font_(font_filepath),
	text_(font_, string, size),
	subtext_(font_, "", size)
{
	font_.setSmooth   (smooth);
	text_.setFillColor(color);	

	bounds_.size = text_.getGlobalBounds().size;
}

void ni::Text::SetTextOutline(int size, sf::Color color)
{
	text_.setOutlineThickness(size);
	text_.setOutlineColor    (color);
}

void ni::Text::Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store)
{
	bounds_.size = text_.getGlobalBounds().size;

	text_.setPosition(bounds_.position);

	target.draw(text_, states);
}
