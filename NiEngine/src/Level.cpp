#include <NiEngine/Level.h>

#include <id.h>

#include <filesystem>
#include <string>
#include <vector>
#include <utility>
#include <format>

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <NiEngine/TilesetBlueprint.h>
#include <NiEngine/TilesetReference.h>
#include <NiEngine/FileUtility.h>
#include <NiEngine/DataHandler.h>
#include <NiEngine/LevelBlueprint.h>
#include <NiEngine/LayerBlueprint.h>
#include <NiEngine/BitmapStore.h>
#include <NiEngine/Tilemap.h>
#include <NiEngine/ObjectFactory.h>
#include <NiEngine/GameMode.h>

void ni::Level::SetTotalLevelCount(int count)
{
	num_of_levels_ = count;
}

void ni::Level::ReloadLevel(GameMode& mode)
{
	tilemap_.Clear();
	mode.GetComponentStore().Clear();
	tileset_blueprints_.clear();
	current_level_blueprint_ = {};

	LoadLevel(current_level_);
	LoadObjects(mode);
}

void ni::Level::LoadNextLevel(GameMode& mode)
{
	if (current_level_ == num_of_levels_)
	{
		last_level_finished_.Notify();
		return;
	}
	current_level_++;
	ReloadLevel(mode);
}

void ni::Level::LoadLevelByIndex(GameMode& mode, int index)
{
	if (index > num_of_levels_)
	{
		return;
	}	
	current_level_ = index;
	ReloadLevel(mode);
}

void ni::Level::EnableTilemapCollisions(b2WorldId world_id)
{
	tilemap_.EnableCollision(world_id);
}

const ni::Tilemap& ni::Level::GetCurrentTilemap() const
{
	return tilemap_;
}

const ni::LayerBlueprint* ni::Level::GetLayerByName(const std::string& layer_name) const
{
	for (auto& layer : current_level_blueprint_.layers_)
	{
		if (layer.name_ == layer_name)
		{
			return &layer;
		}
	}
	return nullptr;
}

void ni::Level::RenderTilemap(sf::RenderTarget& target, sf::RenderStates states, BitmapStore& store)
{
	tilemap_.Render(target, states, store);
}

void ni::Level::LoadLevel(int index)
{
	std::string path = std::format("{}/level_{}/level_{}.json", kDefaultLevelsDirectory, index, index);
	DataHandler<LevelBlueprint> handler(path);

	current_level_blueprint_ = handler.GetBlueprint();

	LoadTilesetBlueprints(current_level_blueprint_.tileset_references_);
	
	tilemap_.Init(current_level_blueprint_.map_size_, current_level_blueprint_.tile_size_);

	for (int i = 0; i < current_level_blueprint_.layers_.size(); ++i)
	{
		LayerBlueprint& layer_blueprint = current_level_blueprint_.layers_[i];
		if (layer_blueprint.name_ == kPrototypeLayerName || layer_blueprint.name_ == kIgnoreLayerName || layer_blueprint.type_ == kObjectsLayerType)
		{
			continue;
		}
		tilemap_.LoadTiles(layer_blueprint, tileset_blueprints_);
	}
}

void ni::Level::RegisterObjectFactory(std::unique_ptr<ObjectFactory> factory)
{
	factory_ = std::move(factory);
}

void ni::Level::LoadTilesetBlueprints(const std::vector<TilesetReference>&tileset_references)
{
	for (auto& tileset_reference : tileset_references)
	{
		std::string tileset_filepath = std::filesystem::path(tileset_reference.file_source_).replace_extension(".json").string();
		tileset_filepath = FileUtility::RemoveRelativePaths(tileset_filepath);

		DataHandler<TilesetBlueprint> tileset_handler(tileset_filepath);
		TilesetBlueprint tileset_blueprint = tileset_handler.GetBlueprint();

		tileset_blueprint.texture_key_ = FileUtility::RemoveRelativePaths(tileset_blueprint.texture_key_);
		tileset_blueprint.first_gid_ = tileset_reference.first_gid_;

		tileset_blueprints_.emplace_back(tileset_blueprint);
	}
}

void ni::Level::LoadObjects(GameMode& mode)
{
	for (auto& object_layer : current_level_blueprint_.object_layers_)
	{
		factory_->LoadObjects(object_layer, tileset_blueprints_, mode);
	}
}
