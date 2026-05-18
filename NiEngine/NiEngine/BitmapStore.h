#pragma once

#include <string>
#include <unordered_map>

#include <SFML/Graphics/Texture.hpp>

class BitmapStore
{
private:
	std::unordered_map<std::string, sf::Texture> map_;

	static bool instantiated_;

public:
	BitmapStore();
	sf::Texture& GetTexture(const std::string& key);
};

