#include <NiEngine/AnimatedGraphicsComponent.h>

#include <string>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <NiEngine/StandardGraphicsComponent.h>
#include <NiEngine/BitmapStore.h>
#include <NiEngine/Engine.h>
#include <NiEngine/Animation.h>
#include <NiEngine/TilemapUtility.h>

ni::AnimatedGraphicsComponent::AnimatedGraphicsComponent(std::string texture_key, sf::Vector2i sprite_size, int frame_spacing) :
	ni::StandardGraphicsComponent(texture_key, { {0, 0}, sprite_size })
{
	frame_spacing_ = frame_spacing;

	initial_frame_rect_ = current_frame_rect_;
}

void ni::AnimatedGraphicsComponent::RegisterAnimation(Animation new_animation)
{
	animations_[new_animation.key_] = new_animation;
}

void ni::AnimatedGraphicsComponent::Play(std::string animation_key, float delay_in_seconds, bool loop)
{
	if (playing_)
	{
		return;
	}
	auto it = animations_.find(animation_key);
	if (it == animations_.end())
	{
		return;
	}
	Animation animation = it->second;
	playing_ = true;
	loop_    = loop;
	current_animation_ = animation;
	current_frame_	   = animation.start_frame - 1;
	delay_in_seconds_  = delay_in_seconds;

	NextFrame();
}

void ni::AnimatedGraphicsComponent::Stop()
{
	playing_ = false;
}

void ni::AnimatedGraphicsComponent::SetFrame(int row, int index)
{
	current_frame_	   = index - 1;
	current_animation_.animation_row = row;
	current_animation_.frame_count   = index + 1;
	playing_ = false;

	NextFrame();
}

bool ni::AnimatedGraphicsComponent::IsPlaying() const
{
	return playing_;
}

std::string ni::AnimatedGraphicsComponent::GetCurrentAnimationKey() const
{
	return current_animation_.key_;
}

void ni::AnimatedGraphicsComponent::Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store)
{
	float time_elapsed_since_last_frame = (ni::Engine::time_elapsed - time_since_last_animation_frame_).asSeconds();

	if (playing_ && time_elapsed_since_last_frame >= delay_in_seconds_)
	{
		NextFrame();
	}	
	ni::StandardGraphicsComponent::Render(target, states, store);
}

void ni::AnimatedGraphicsComponent::NextFrame()
{
	current_frame_++;
	if (current_frame_ < current_animation_.GetEndFrame())
	{
		current_frame_rect_ = TilemapUtility::GetTextureBounds({current_frame_, current_animation_.animation_row}, current_frame_rect_.size.x, 1);
		
		time_since_last_animation_frame_ = ni::Engine::time_elapsed;
		return;
	}
	if (loop_)
	{
		current_frame_ = current_animation_.start_frame - 1;
		return;
	}
	playing_ = false;
}
