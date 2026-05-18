#include <NiEngine/BitmapStore.h>

#include <cassert>
#include <string>
#include <iostream>
#include <format>

#include <SFML/Graphics/Texture.hpp>

bool BitmapStore::instantiated_ = false;

BitmapStore::BitmapStore()
{
    assert(!instantiated_);
    instantiated_ = true;
}

sf::Texture& BitmapStore::GetTexture(const std::string& key)
{
    auto result = map_.find(key);

    if (result != map_.end())
    {
        return result->second;
    }

    bool success = map_[key].loadFromFile(key);

    if (!success)
    {
#ifdef _DEBUG
        std::cout << std::format("[NiEngine/Bitmap]: Texture not found for key `{}`", key);
#endif // _DEBUG
    }

    return map_[key];
}
