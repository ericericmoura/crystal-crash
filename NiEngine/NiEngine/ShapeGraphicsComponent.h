#pragma once

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <NiEngine/BitmapStore.h>
#include <NiEngine/GraphicsComponent.h>

namespace ni {

template <typename TShape>
class ShapeGraphicsComponent : public GraphicsComponent
{
private:
	TShape shape_;

public:
	ShapeGraphicsComponent(TShape shape);

	void Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store) override;
};

template<typename TShape>
inline ShapeGraphicsComponent<TShape>::ShapeGraphicsComponent(TShape shape)
{
	shape_ = shape;
}

template<typename TShape>
inline void ShapeGraphicsComponent<TShape>::Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store)
{
	target.draw(shape_, states);
}

}