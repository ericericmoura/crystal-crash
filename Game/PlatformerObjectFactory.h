#pragma once

#include <vector>
#include <string>

#include <NiEngine/ObjectFactory.h>
#include <NiEngine/TilesetBlueprint.h>
#include <NiEngine/GameMode.h>
#include <NiEngine/ObjectTemplateBlueprint.h>
#include <NiEngine/ObjectBlueprint.h>
#include <SFML/Graphics/Rect.hpp>

enum ObjectTypes
{
	None      = 0,
	Slingshot = 1
};

class PlatformerObjectFactory : public ni::ObjectFactory
{
private:
	void SpawnObject   (ni::ObjectBlueprint object, ni::ObjectTemplateBlueprint& object_template, const std::vector<ni::TilesetBlueprint>& tileset_blueprints, ni::GameMode& mode, int type) override;
	void SpawnSlingshot(ni::ObjectBlueprint object, ni::ObjectTemplateBlueprint& object_template, std::string texture_key, sf::IntRect texture_coordinates, ni::GameMode& mode);

	template <typename T>
	T GetAttributeFromObject(ni::ObjectBlueprint& object, ni::ObjectTemplateBlueprint& object_template, const std::string& attribute)
	{
		T result = object_template.properties_map_.at(attribute).GetValue<T>();

		if (object.properties_.contains(attribute)) result = object.properties_.at(attribute).GetValue<T>();

		return result;
	}
};

