#include "TrailGraphicsComponent.h"

#include <iostream>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <NiEngine/BitmapStore.h>

TrailGraphicsComponent::TrailGraphicsComponent()
{
	vertices_.setPrimitiveType(sf::PrimitiveType::Triangles);

	if (!opacity_shader_.loadFromFile("shaders/opacity.frag", sf::Shader::Type::Fragment))
	{
#ifdef _DEBUG
		std::cout << "\nError while loading opacity shader!";
#endif // _DEBUG
	}
}

void TrailGraphicsComponent::SpawnParticle(sf::Vector2f position, int size)
{
	float half_size = size/2.0f;

	sf::Color particle_color = sf::Color::White;

	vertices_.append({ position + sf::Vector2f(-half_size, -half_size), particle_color });
	vertices_.append({ position + sf::Vector2f( half_size, -half_size), particle_color });
	vertices_.append({ position + sf::Vector2f(-half_size,  half_size), particle_color });

	vertices_.append({ position + sf::Vector2f( half_size, -half_size), particle_color });
	vertices_.append({ position + sf::Vector2f( half_size,  half_size), particle_color });
	vertices_.append({ position + sf::Vector2f(-half_size,  half_size), particle_color });
}

void TrailGraphicsComponent::Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store)
{	
	opacity_shader_.setUniform("alpha", alpha_);
	states.shader  = &opacity_shader_;
	states.texture = nullptr;
	target.draw(vertices_, states);
}
