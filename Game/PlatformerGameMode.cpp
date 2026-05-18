#include "PlatformerGameMode.h"

#include <memory>
#include <utility>
#include <format>
#include <string>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Color.hpp>
#include <NiEngine/Converter.h>
#include <NiEngine/BitmapStore.h>
#include <NiEngine/GameMode.h>
#include <NiEngine/WipeScreenTransition.h>
#include <NiEngine/GameModeController.h>
#include <NiEngine/TextFadeScreenTransition.h>
#include <NiEngine/Text.h>
#include <NiEngine/HUDComponent.h>
#include <NiEngine/ServiceLocator.h>
#include <NiEngine/DataHandler.h>

#include "LevelStartBlueprint.h"
#include "PlatformerObjectFactory.h"

PlatformerGameMode::PlatformerGameMode() : hud_(sf::Color::Black, {20, 0}, { 20, 15 }, false, 2, {0, 0})
{	
	ni::DataHandler<LevelStartBlueprint> config_handler("maps/config.json");
	ni::Converter::pixels_per_meters_ = 16;

	LevelStartBlueprint config = config_handler.GetBlueprint();

	auto level_text = std::make_unique<ni::Text>(kMainGameFontKey, std::format("Level {}", config.start_level_), sf::Color::White, 30);
	level_text->SetTextOutline(2, sf::Color::Black);
	int text_component_index = hud_.AddComponent(std::move(level_text));

	auto deaths_text = std::make_unique<ni::Text>(kMainGameFontKey, "Deaths 0", sf::Color::White, 30);
	deaths_text->SetTextOutline(2, sf::Color::Black);
	int death_text_component_index = hud_.AddComponent(std::move(deaths_text));

	auto factory = std::make_unique<PlatformerObjectFactory>();
	level_.RegisterObjectFactory(std::move(factory));
	level_.SetTotalLevelCount(config.total_level_count_);
	level_.LoadLevelByIndex(*this, config.start_level_);
	world_camera_.FitTo(level_.GetCurrentTilemap().GetBounds());

	game_over_transition_   .Init(2, "Game   Over!"                      , kMainGameFontKey, 50, sf::Color::White, sf::Color::Black, transitions_camera_.GetView().getSize());
	
	if (!config.skip_intro_)
	{
		engine_title_transition_.Init(2, "\t\t NI   Engine\nPor  Eric  Moura", kMainGameFontKey, 50, sf::Color::White, sf::Color::Black, transitions_camera_.GetView().getSize());
		engine_title_transition_.OnTransitionFinished([this]() {
			current_transition_->Play(true);
			ni::ServiceLocator::Instance().GetSoundEngine().PlayMusic(kMainMusicKey, true, 1);	
		});
		engine_title_transition_.Play();
	}
	else
	{
		ni::ServiceLocator::Instance().GetSoundEngine().PlayMusic(kMainMusicKey, true, 1);	
	}
	
	current_transition_ = std::make_unique<ni::WipeScreenTransition>(.8f, transitions_camera_.GetView().getSize(), false, sf::Color::Black);
	current_transition_->OnTransitionCoveredScreen([this, text_component_index, death_text_component_index]() {
		if (load_next_level_)
		{			
			level_.LoadNextLevel(*this);
			load_next_level_ = false;

			auto text_component = GetLevelTextHUD(text_component_index);
			if (!text_component)
			{
				return;
			}
			std::string level_string = std::format("Level {}", level_.GetCurrentLevelIndex());
			text_component->SetTextString(level_string);
		}
		else if (restart_level_)
		{
			level_.ReloadLevel(*this);
			restart_level_ = false;

			player_death_counter_++;

			auto text_component = GetLevelTextHUD(death_text_component_index);
			if (!text_component)
			{
				return;
			}
			std::string level_string = std::format("Deaths {}", player_death_counter_);
			text_component->SetTextString(level_string);
			return;
		}
		restart_level_ = false;
		load_next_level_ = false;
	});	
	current_transition_->OnTransitionFinished([this]() {
		transitioning_ = false;
	});
	level_.OnLastLevelFinished([this]() {
		paused_ = true;
		game_over_transition_.Play();
		game_over_transition_.StopHalfway();
	});
}

void PlatformerGameMode::PrepareToLoadNextLevel()
{
	load_next_level_ = true;
}

void PlatformerGameMode::RestartLevel()
{
	restart_level_ = true;
}

void PlatformerGameMode::Update(ni::GameModeController& controller)
{
	if ((restart_level_ || load_next_level_) && !transitioning_)
	{		
		current_transition_->Play();
		transitioning_ = true;
		return;
	}
	GameMode::Update(controller);

	game_over_transition_.Update();
}

void PlatformerGameMode::Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store)
{
	world_camera_.ApplyTo(target);

	level_.RenderTilemap(target, states, store);
	component_store_.Render(target, states, store);

	hud_camera_.ApplyTo(target);
	hud_.Render(target, states, store);

	transitions_camera_.ApplyTo(target);

	if (current_transition_)
	{
		current_transition_->Render(target, states, store);
	}
	engine_title_transition_.Render(target, states, store);	
	game_over_transition_   .Render(target, states, store);
}

ni::Text* PlatformerGameMode::GetLevelTextHUD(int component_index) const
{
	ni::HUDComponent* hud_component = hud_.GetComponentByIndex(component_index);
	if (!hud_component)
	{
		return nullptr;
	}
	auto text_component = static_cast<ni::Text*>(hud_component);
	return text_component;
}
