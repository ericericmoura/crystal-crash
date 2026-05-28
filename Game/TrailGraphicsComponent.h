#pragma once

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <NiEngine/GraphicsComponent.h>
#include <NiEngine/BitmapStore.h>

class TrailGraphicsComponent : public ni::GraphicsComponent
{
public:
	TrailGraphicsComponent();

	void SpawnParticle(sf::Vector2f position);
	void Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store) override;

private:
	sf::VertexArray vertices_ = {};	
};

