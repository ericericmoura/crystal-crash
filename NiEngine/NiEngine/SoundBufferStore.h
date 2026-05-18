#pragma once

#include <string>
#include <unordered_map>

#include <SFML/Audio/SoundBuffer.hpp>

namespace ni {

class SoundBufferStore
{
public:
	void Preload(const std::string& key);
	sf::SoundBuffer& GetSoundBuffer(const std::string& key);

private:
	std::unordered_map<std::string, sf::SoundBuffer> sound_buffers_;
};

}