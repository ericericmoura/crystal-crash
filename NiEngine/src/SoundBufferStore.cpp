#include <NiEngine/SoundBufferStore.h>

#include <iostream>
#include <format>
#include <string>

#include <SFML/Audio/SoundBuffer.hpp>

void ni::SoundBufferStore::Preload(const std::string& key)
{
	GetSoundBuffer(key);
}

sf::SoundBuffer& ni::SoundBufferStore::GetSoundBuffer(const std::string& key)
{
	auto it = sound_buffers_.find(key);
	if (it != sound_buffers_.end())
	{
		return it->second;
	}
	bool success = sound_buffers_[key].loadFromFile(key);
	if (!success)
	{
	#ifdef _DEBUG
		std::cout << std::format("[NiEngine/SoundBufferStore]: Sound file not found or incompatible for key `{}`", key);
	#endif // _DEBUG
	}
	return sound_buffers_[key];
}
