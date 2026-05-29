#include "TrailGraphicsComponent.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <NiEngine/BitmapStore.h>

TrailGraphicsComponent::TrailGraphicsComponent()
{
	vertices_.setPrimitiveType(sf::PrimitiveType::Points);
}

void TrailGraphicsComponent::SpawnParticle(sf::Vector2f position)
{
	sf::Vertex vertex{};
	vertex.position = position;
	vertex.color = sf::Color::White;
	vertices_.append(vertex);
}

void TrailGraphicsComponent::Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store)
{	
	states.texture = nullptr;

	target.draw(vertices_, states);
}
