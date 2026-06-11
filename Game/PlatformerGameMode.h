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
	inline static std::string kSlingshotAmmoTag = "slingshot_ammo";
	inline static std::string kObstacleTag      = "obstacle";

	PlatformerGameMode();

	void RestartLevel();
	void PrepareToLoadNextLevel();

	virtual void PhysicsUpdate(float delta) override;
	virtual void Update(ni::GameModeController& controller) override;
	virtual void Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store) override;

private:	
	inline static std::string kMainGameFontKey = "fonts/ARCADECLASSIC.TTF";

	bool transitioning_   = false;
	bool restart_level_   = false;
	bool load_next_level_ = false;

	ni::TextFadeScreenTransition game_over_transition_;

	ni::Panel hud_;

	ni::Text* GetLevelTextHUD(int component_index) const;

	void HandleCollisions();
};

