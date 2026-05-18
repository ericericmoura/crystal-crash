#pragma once

#include <string>
#include <unordered_map>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include "Animation.h"
#include "StandardGraphicsComponent.h"
#include "BitmapStore.h"

namespace ni {

class AnimatedGraphicsComponent : public StandardGraphicsComponent
{
public:
	AnimatedGraphicsComponent(std::string texture_key, sf::Vector2i sprite_size, int frame_spacing = 0);

	void RegisterAnimation(Animation new_animation);
	void Play(std::string animation_key, float delay_in_seconds, bool loop = false);

	void Stop();
	void SetFrame(int row, int index);
	bool IsPlaying() const;
	
	std::string GetCurrentAnimationKey() const;

	virtual void Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store) override;

private:
	std::unordered_map<std::string, Animation> animations_;

	Animation current_animation_ = {};

	sf::IntRect initial_frame_rect_;

	int animation_count_ = 0;
	int frame_spacing_   = 0;

	int current_frame_	   = 0;

	bool loop_    = false;
	bool playing_ = false;

	float delay_in_seconds_ = 0;
	sf::Time time_since_last_animation_frame_;

	void NextFrame();
};

}