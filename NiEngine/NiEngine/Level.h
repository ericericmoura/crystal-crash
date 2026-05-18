#pragma once

#include <id.h>

#include <vector>
#include <string>
#include <memory>
#include <functional>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "TilesetBlueprint.h"
#include "Tilemap.h"
#include "ObjectFactory.h"
#include "LevelBlueprint.h"
#include "LayerBlueprint.h"
#include "TilesetReference.h"
#include "BitmapStore.h"
#include "Subject.h"

namespace ni {

class GameMode;

class Level
{
public:
	inline static const std::string kDefaultLevelsDirectory      = "maps";
	inline static const std::string kTransparentTilesTilesetName = "transparent_tiles";

	inline static const std::string kPrototypeLayerName = "prototype";
	inline static const std::string kTerrainLayerName   = "terrain";
	inline static const std::string kIgnoreLayerName    = "gameignore";
	inline static const std::string kObjectsLayerType   = "objectgroup";

	void SetTotalLevelCount(int count);

	void ReloadLevel  (GameMode& mode);
	void LoadNextLevel(GameMode& mode);
	void LoadLevelByIndex(GameMode& mode, int index);
	void RegisterObjectFactory(std::unique_ptr<ObjectFactory> factory);

	int GetCurrentLevelIndex() const { return current_level_; }

	int OnLastLevelFinished(std::function<void()> callback) { return last_level_finished_.Subscribe(callback); };

	void EnableTilemapCollisions(b2WorldId world_id);

	const Tilemap& GetCurrentTilemap() const;
	const LayerBlueprint* GetLayerByName(const std::string& layer_name) const;

	void RenderTilemap(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store);

private:
	int current_level_ = 0;
	int num_of_levels_ = 0;
	
	Subject<> last_level_finished_;

	LevelBlueprint current_level_blueprint_;
	std::vector<TilesetBlueprint> tileset_blueprints_ = {};

	std::string last_loaded_file_ = "";

	Tilemap tilemap_;
	std::unique_ptr<ObjectFactory> factory_;

	void LoadTilesetBlueprints(const std::vector<TilesetReference>& tileset_references);
	void LoadObjects(GameMode& mode);
	void LoadLevel(int index);
};

}