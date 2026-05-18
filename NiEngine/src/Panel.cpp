#include <NiEngine/Panel.h>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <NiEngine/BitmapStore.h>
#include <NiEngine/Container.h>

ni::Panel::Panel(sf::Color background_color, sf::Vector2f gap, sf::Vector2f margin, bool vertical, int max_columns, sf::FloatRect rect) : Container(gap, margin, vertical, max_columns)
{	
	bounds_.size = rect.size;

	rect.size += margin;
	background_.setPosition(rect.position);
	background_.setSize    (rect.size);	

	Panel(background_color, gap, margin, vertical, max_columns, rect.position);
}

ni::Panel::Panel(sf::Color background_color, sf::Vector2f gap, sf::Vector2f margin, bool vertical, int max_columns, sf::Vector2f position) : Container(gap, margin, vertical, max_columns)
{
	bounds_.position = position;

	background_.setFillColor(background_color);
}

void ni::Panel::Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store)
{
	background_.setPosition(GetBounds().position);
	background_.setSize(GetBounds().size);

	target.draw(background_, states);
	Container::Render(target, states, store);
}
