#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/System/Time.hpp>
#include <NiEngine/BitmapStore.h>
#include <NiEngine/Subject.h>
#include <functional>

namespace ni {

class ScreenTransition
{
public:
	ScreenTransition() = default;
	ScreenTransition(float delay_in_seconds) { delay_in_seconds_ = delay_in_seconds;  };

	void Play(bool start_reversed = false) 
	{ 
		playing_ = true; 
		time_since_start_ = GetTimeElapsed();
		playing_reversed_ = start_reversed;

		Prepare();
	};

	void Stop() 
	{ 
		playing_ = false; 
	};

	void OnTransitionFinished     (std::function<void()> callback) { on_transition_finished_.Subscribe(callback);       };
	void OnTransitionCoveredScreen(std::function<void()> callback) { on_transition_covered_screen_.Subscribe(callback); };
	
	virtual void Update() = 0;
	virtual void Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store) = 0;

protected:
	float    delay_in_seconds_ = 0;
	bool     playing_          = false;
	bool     playing_reversed_ = false;

	sf::Time time_since_start_ = {};
	
	Subject<> on_transition_finished_;
	Subject<> on_transition_covered_screen_;

private:
	sf::Time GetTimeElapsed() const;
	
	virtual void Prepare() {};
};

}