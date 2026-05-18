#pragma once

#include <string>
#include <queue>
#include <unordered_map>

#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>

#include "SoundBufferStore.h"

namespace ni {

class SoundEngine
{

friend class Engine;

public:
	void Preload(const std::string& key);
	void PlaySound(std::string key, float volume = 1);
	void FlushSoundQueue();
	
	void PlayMusic(const std::string& key, bool loop = true, float volume = 1);

private:
	inline static const int kMaxSoundsPerFrame = 16;

	struct SoundMessage
	{
		std::string key = "";
		float volume = 1;
	};

	SoundBufferStore sound_buffer_store_;

	std::queue<SoundMessage> sound_queue_;
	std::unordered_map<std::string, sf::Sound*> sounds_map_;
	std::unordered_map<std::string, sf::Music*> musics_map_;
};

}