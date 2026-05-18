#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include "ObjectLayerBlueprint.h"
#include "ObjectTemplateBlueprint.h"
#include "TilesetBlueprint.h"
#include "ObjectBlueprint.h"

namespace ni {

class GameMode;

class ObjectFactory
{
public:
	void LoadObjects(ObjectLayerBlueprint& layer_blueprint, const std::vector<TilesetBlueprint>& tileset_blueprints, GameMode& mode);
	virtual void SpawnObject(ni::ObjectBlueprint object, ni::ObjectTemplateBlueprint& object_template, const std::vector<ni::TilesetBlueprint>& tileset_blueprints, ni::GameMode& mode, int type) = 0;

private:
	std::unordered_map<std::string, ni::ObjectTemplateBlueprint> template_map_;
};

}