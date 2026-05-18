#include <NiEngine/Camera.h>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Rect.hpp>

sf::Vector2f ni::Camera::GetCoordinatesFromPixel(sf::RenderTarget& target, const sf::Vector2i& position) const
{
	return target.mapPixelToCoords(position, view_);
}

void ni::Camera::FitTo(sf::FloatRect bounds)
{	
	view_.setSize(bounds.size);

	SetPosition(bounds.position);
}

void ni::Camera::SetPosition(sf::Vector2f position)
{
	view_.setCenter({
			position.x + view_.getSize().x / 2,
			position.y + view_.getSize().y / 2
	});
}

void ni::Camera::ApplyTo(sf::RenderTarget& target)
{
	target.setView(view_);
}
