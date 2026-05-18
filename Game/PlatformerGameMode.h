#pragma once

#include <string>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <NiEngine/GameMode.h>
#include <NiEngine/BitmapStore.h>
#include <NiEngine/GameModeController.h>
#include <NiEngine/Panel.h>
#include <NiEngine/Text.h>
#include <NiEngine/TextFadeScreenTransition.h>

class PlatformerGameMode : public ni::GameMode
{
public:
	inline static const std::string kSpikeTag  = "spike";
	inline static const std::string kPlayerTag = "player";
	inline static const std::string kEnemyTag  = "enemy";

	PlatformerGameMode();

	void RestartLevel();
	void PrepareToLoadNextLevel();
	virtual void Update(ni::GameModeController& controller) override;
	virtual void Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store) override;

private:	
	inline static std::string kMainGameFontKey = "fonts/ARCADECLASSIC.TTF";
	inline static std::string kMainMusicKey    = "sounds/main_music.wav";

	bool transitioning_   = false;
	bool restart_level_   = false;
	bool load_next_level_ = false;

	int player_death_counter_ = 0;

	ni::TextFadeScreenTransition game_over_transition_;

	ni::Panel hud_;

	ni::Text* GetLevelTextHUD(int component_index) const;
};

