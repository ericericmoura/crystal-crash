#include <NiEngine/HitboxComponent.h>

#include <SFML/System/Vector2.hpp>

void ni::HitboxComponent::SetPositionOffset(sf::Vector2f offset)
{
	position_offset_ = offset;
}

void ni::HitboxComponent::SetPosition(sf::Vector2f position)
{
	box_.position = position + position_offset_;
}

void ni::HitboxComponent::SetCenterPosition(sf::Vector2f position)
{
	box_.position = position - box_.size/2.0f;
}

void ni::HitboxComponent::SetSize(sf::Vector2f size)
{
	box_.size = size;
}

bool ni::HitboxComponent::IsColliding(const HitboxComponent& other)
{
	if (box_.findIntersection(other.box_))
	{
		return true;
	}
	return false;
}
