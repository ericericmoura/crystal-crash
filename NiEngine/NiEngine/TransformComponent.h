#pragma once

#include <math_functions.h>

#include <SFML/Graphics/Transformable.hpp>

#include <NiEngine/Component.h>

namespace ni {

class TransformComponent : public Component
{
private:
	sf::Transformable transformable_;

public:
	sf::Transformable& GetTransformable()
	{
		return transformable_;
	}

	void SetPositionInMeters(b2Vec2 position);
	b2Vec2 GetPositionInMeters() const;

	void  SetRotation(float radians);
	float GetRotationInRadians() const;
};

}