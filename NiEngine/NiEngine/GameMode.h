#pragma once

#include <memory>

#include <SFML/Graphics/RenderTarget.hpp>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <NiEngine/ComponentStore.h>
#include <NiEngine/Id.h>
#include <NiEngine/GameObjectTag.h>
#include <NiEngine/Level.h>
#include <NiEngine/PhysicsEngine.h>
#include <NiEngine/ScreenTransition.h>
#include <NiEngine/TextFadeScreenTransition.h>
#include <NiEngine/Camera.h>

#include "BitmapStore.h"

namespace ni {

class GameModeController;

class GameMode
{
private:
	inline static sf::Vector2f mouse_position_in_world_coodinates_ = {};

	Id<GameObjectTag> current_game_object_id_{ 0 };

	bool box2d_enabled = false;

protected:
	bool paused_ = false;

	Level level_;
	
	Camera world_camera_;
	Camera hud_camera_;
	Camera transitions_camera_;

	ComponentStore component_store_;
	PhysicsEngine  physics_engine_;
	
	std::unique_ptr<ScreenTransition> current_transition_;

	TextFadeScreenTransition engine_title_transition_;

public:
	GameMode();

	static sf::Vector2f GetMousePositionInWorldCoordinates() 
	{
		return mouse_position_in_world_coodinates_;
	}

	ComponentStore& GetComponentStore()
	{
		return component_store_;
	}

	PhysicsEngine& GetPhysicsEngine()
	{
		return physics_engine_;
	}

	Id<GameObjectTag> CreateGameObject()
	{
		return ++current_game_object_id_;
	}

	void LoadLevel(int index, bool enable_tilemap_collisions = true);
	void LoadNextLevel(bool enable_tilemap_collisions = true);

	void SetBox2dEnabled(bool value)
	{
		box2d_enabled = value;
	}

	virtual void PhysicsUpdate(float delta);
	virtual void Update(GameModeController& controller);
	virtual void Render(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store);
	virtual void SpawnComponents();
};

}