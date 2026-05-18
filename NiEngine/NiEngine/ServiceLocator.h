#pragma once

#include <NiEngine/EventDispatcher.h>

#include "SoundEngine.h"

namespace ni {

class ServiceLocator
{
private:
	EventDispatcher event_dispatcher_;
	SoundEngine     sound_engine_;
	
	ServiceLocator() = default;

	ServiceLocator(const ServiceLocator&) = delete;
	ServiceLocator(ServiceLocator&&)      = delete;

	ServiceLocator& operator=(ServiceLocator&&)      = delete;
	ServiceLocator& operator=(const ServiceLocator&) = delete;

public:
	static ServiceLocator& Instance()
	{
		static ServiceLocator instance;
		return instance;
	}

	EventDispatcher& GetEventDispatcher() 
	{ 
		return event_dispatcher_;
	}

	SoundEngine& GetSoundEngine()
	{
		return sound_engine_;
	}
};

}