#include <NiEngine/TransformComponent.h>

#include <math_functions.h>

#include <SFML/System/Angle.hpp>

#include <NiEngine/Converter.h>

void ni::TransformComponent::SetPositionInMeters(b2Vec2 position)
{
	transformable_.setPosition(Converter::MetersToPixels(position));
}

b2Vec2 ni::TransformComponent::GetPositionInMeters() const
{
	return Converter::PixelsToMeters(transformable_.getPosition());
}

void ni::TransformComponent::SetRotation(float radians)
{
	transformable_.setRotation(sf::radians(radians));
}

float ni::TransformComponent::GetRotationInRadians() const
{
	return transformable_.getRotation().asRadians();
}
