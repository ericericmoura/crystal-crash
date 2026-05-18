#include <NiEngine/ScreenTransition.h>

#include <SFML/System/Time.hpp>
#include <NiEngine/Engine.h>

sf::Time ni::ScreenTransition::GetTimeElapsed() const
{
    return Engine::time_elapsed;
}
