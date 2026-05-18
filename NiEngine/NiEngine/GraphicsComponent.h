#pragma once

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <NiEngine/Component.h>
#include <NiEngine/BitmapStore.h>

namespace ni {

class GraphicsComponent : public Component
{
public:
	virtual void Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store) = 0;
};

}