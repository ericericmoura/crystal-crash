#include "PlatformerGameMode.h"

#include <id.h>
#include <types.h>
#include <box2d.h>

#include <memory>
#include <utility>
#include <format>
#include <string>
#include <cstdarg>
#include <cstdint>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Color.hpp>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Id.h>
#include <NiEngine/UpdateComponent.h>
#include <NiEngine/Converter.h>
#include <NiEngine/BitmapStore.h>
#include <NiEngine/GameMode.h>
#include <NiEngine/WipeScreenTransition.h>
#include <NiEngine/GameModeController.h>
#include <NiEngine/TextFadeScreenTransition.h>
#include <NiEngine/Text.h>
#include <NiEngine/HUDComponent.h>
#include <NiEngine/DataHandler.h>

#include "LevelStartBlueprint.h"
#include "PlatformerObjectFactory.h"
#include "ObstacleUpdateComponent.h"

PlatformerGameMode::PlatformerGameMode() : hud_(sf::Color::Black, {20, 0}, { 20, 15 }, false, 2, {0, 0})
{		
	// Map configuration
	ni::DataHandler<LevelStartBlueprint> config_handler("maps/config.json");
	ni::Converter::pixels_per_meters_ = 32;

	LevelStartBlueprint config = config_handler.GetBlueprint();

	// UI configuration
	auto level_text = std::make_unique<ni::Text>(kMainGameFontKey, std::format("Level {}", config.start_level_), sf::Color::White, 30);
	level_text->SetTextOutline(2, sf::Color::Black);
	int text_component_index = hud_.AddComponent(std::move(level_text));

	auto deaths_text = std::make_unique<ni::Text>(kMainGameFontKey, "Deaths 0", sf::Color::White, 30);
	deaths_text->SetTextOutline(2, sf::Color::Black);
	int death_text_component_index = hud_.AddComponent(std::move(deaths_text));

	// Physics configuration

	SetBox2dEnabled(true);

	b2WorldDef world_def = b2DefaultWorldDef();
	world_def.gravity = { 0, 9.8f };

	b2WorldId world_id = b2CreateWorld(&world_def);

	GetPhysicsEngine().CreateWorld(world_def);

	auto factory = std::make_unique<PlatformerObjectFactory>();
	factory->SetWorldId(GetPhysicsEngine().GetWorldId());

	level_.EnableTilemapCollisions(GetPhysicsEngine().GetWorldId());
	level_.RegisterObjectFactory(std::move(factory));
	level_.SetTotalLevelCount   (config.total_level_count_);
	level_.LoadLevelByIndex     (*this, config.start_level_);
	world_camera_.FitTo(level_.GetCurrentTilemap().GetBounds());

	level_.OnLastLevelFinished([this]() {
		paused_ = true;
		game_over_transition_.Play();
		game_over_transition_.StopHalfway();
	});

	// Transition config
	game_over_transition_   .Init(2, "Game   Over!", kMainGameFontKey, 50, sf::Color::White, sf::Color::Black, transitions_camera_.GetView().getSize());
	
	if (!config.skip_intro_)
	{
		engine_title_transition_.Init(2, "\t\t NI   Engine\nPor  Eric  Moura", kMainGameFontKey, 50, sf::Color::White, sf::Color::Black, transitions_camera_.GetView().getSize());
		engine_title_transition_.OnTransitionFinished([this]() {
			current_transition_->Play(true);
		});
		engine_title_transition_.Play();
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
			return;
		}
		restart_level_ = false;
		load_next_level_ = false;
	});	
	current_transition_->OnTransitionFinished([this]() {
		transitioning_ = false;
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

void PlatformerGameMode::PhysicsUpdate(float delta)
{
	if (paused_)
	{
		return;
	}
	if (box2d_enabled_)
	{
		physics_engine_.PhysicsUpdate();

		HandleCollisions();
	}
	component_store_.PhysicsUpdate(physics_engine_.GetWorldId(), level_.GetCurrentTilemap(), delta);
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
	ni::GameMode::Render(target, states, store);

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

void PlatformerGameMode::HandleCollisions()
{
	b2ContactEvents contactEvents = b2World_GetContactEvents(physics_engine_.GetWorldId());
	for (int i = 0; i < contactEvents.hitCount; ++i)
	{
		b2ContactHitEvent* hit_event = contactEvents.hitEvents + i;
		if (hit_event->approachSpeed > 10.0f)
		{
			ni::Id<ni::GameObjectTag> body_a_id;
			b2BodyId body_a = b2Shape_GetBody(hit_event->shapeIdA);
			body_a_id.id_ = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(b2Body_GetUserData(body_a)));

			ni::Id<ni::GameObjectTag> body_b_id;
			b2BodyId body_b = b2Shape_GetBody(hit_event->shapeIdB);
			body_b_id.id_ = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(b2Body_GetUserData(body_b)));

			ni::UpdateComponent* update_a = component_store_.GetUpdateComponent(body_a_id);
			ni::UpdateComponent* update_b = component_store_.GetUpdateComponent(body_b_id);

			if (!update_a || !update_b)
			{
				continue;
			}

			if (update_a->unique_tag_ == PlatformerGameMode::kObstacleTag)
			{
				static_cast<ObstacleUpdateComponent*>(update_a)->Damage(25);
			}
			else if (update_b->unique_tag_ == PlatformerGameMode::kObstacleTag)
			{
				static_cast<ObstacleUpdateComponent*>(update_b)->Damage(25);
			}
		}
	}
}
