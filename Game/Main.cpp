#include <memory>
#include <utility>

#include <SFML/Window/WindowEnums.hpp>
#include <NiEngine/Engine.h>

#include "PlatformerGameMode.h"

int main()
{
    ni::Engine engine("Cat Mario clone, by Eric", sf::State::Fullscreen);

    auto mode = std::make_unique<PlatformerGameMode>();
    engine.GetGameModeController().Register(std::move(mode));

    engine.Run();
}