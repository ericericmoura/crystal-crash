#include <NiEngine/Container.h>

#include <memory>
#include <utility>
#include <algorithm>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <NiEngine/BitmapStore.h>
#include <NiEngine/HUDComponent.h>

ni::Container::Container(sf::Vector2f gap, sf::Vector2f margin, bool vertical, int max_columns)
{
	gap_         = gap;
	margin_      = margin;
	vertical_    = vertical;
	max_columns_ = max_columns;
}

int ni::Container::AddComponent(std::unique_ptr<HUDComponent> component)
{
	hud_components_.push_back(std::move(component));

	return hud_components_.size() - 1;
}

ni::HUDComponent* ni::Container::GetComponentByIndex(int index) const
{
	if (index < 0 || index >= hud_components_.size())
	{
		return nullptr;
	}
	return hud_components_.at(index).get();
}

void ni::Container::Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store)
{
    float previous_width_count = 0;
    float total_height = 0;
    float max_width = 0;
    float tallest_cell_height = 0;
    int   previous_row = 0;

    for (int i = 0; i < static_cast<int>(hud_components_.size()); ++i)
    {
        auto& component = hud_components_.at(i);

        int grid_x = i % max_columns_;
        int grid_y = i / max_columns_;

        if (previous_row != grid_y)
        {
            previous_row = grid_y;

            float row_width = previous_width_count + (max_columns_ - 1) * gap_.x;
            max_width = std::max(max_width, row_width);
            previous_width_count = 0;

            total_height += tallest_cell_height + gap_.y;
            tallest_cell_height = 0;
        }

        sf::Vector2f position = GetBounds().position;
        position.x += previous_width_count + grid_x * gap_.x + margin_.x;
        position.y += total_height + margin_.y;

        component->GetBounds().position = position;
        component->Render(target, states, store);

        previous_width_count += component->GetBounds().size.x;
        tallest_cell_height = std::max(tallest_cell_height, component->GetBounds().size.y);
    }

    if (max_width == 0)
    {
        int last_row_cols = max_columns_;
        max_width = previous_width_count + (last_row_cols - 1) * gap_.x;
    }
    
    total_height += tallest_cell_height;

    container_size_ = { max_width, total_height };

    if (fit_content_)
    {
        GetBounds().size = container_size_ + margin_ * 2.f;
    }
}