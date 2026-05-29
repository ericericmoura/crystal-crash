#pragma once

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/System/Vector2.hpp>
#include <NiEngine/GraphicsComponent.h>
#include <NiEngine/BitmapStore.h>

class TrailGraphicsComponent : public ni::GraphicsComponent
{
public:
	TrailGraphicsComponent();

	void SetAlpha(float alpha)
	{
		alpha_ = alpha;
	}

	void SpawnParticle(sf::Vector2f position, int size = 3);
	void Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store) override;

private:
	sf::VertexArray vertices_ = {};

	sf::Shader opacity_shader_ = {};
	float alpha_ = 1.0f;
};

