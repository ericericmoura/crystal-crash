#pragma once

#include <memory>
#include <vector>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "HUDComponent.h"
#include "BitmapStore.h"

namespace ni {

class Container : public HUDComponent
{
public:
	Container(sf::Vector2f gap, sf::Vector2f margin, bool vertical, int max_columns);
	int AddComponent(std::unique_ptr<HUDComponent> component);
	HUDComponent* GetComponentByIndex(int index) const;

	sf::Vector2f GetContainerSize() const { return container_size_; }

	void Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store) override;

private:
	std::vector<std::unique_ptr<HUDComponent>> hud_components_ = {};

	sf::Vector2f container_size_ = {};

	sf::Vector2f gap_    = {};
	sf::Vector2f margin_ = {};
	int  max_columns_ = 0;
	bool vertical_    = false;

	bool fit_content_ = true;
};

}