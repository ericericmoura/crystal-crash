#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

namespace ni {

class HitboxComponent
{
public:
	void SetPositionOffset(sf::Vector2f offset  );
	void SetPosition      (sf::Vector2f position);
	void SetCenterPosition(sf::Vector2f position);
	void SetSize          (sf::Vector2f size    );

	bool IsColliding(const HitboxComponent& other);

private:
	sf::FloatRect box_;
	sf::Vector2f  position_offset_;

};

}