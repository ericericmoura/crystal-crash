#pragma once

#include <cstdint>

#include <SFML/System/Vector2.hpp>

namespace ni {

struct Vector2iHash
{
    size_t operator()(const sf::Vector2i& v) const
    {
        return ((uint64_t)(uint32_t)v.x << 32) | (uint32_t)v.y;
    }
};

}