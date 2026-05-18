#pragma once

#include <vector>
#include <string>

#include <SFML/Graphics/Rect.hpp>
#include <NiEngine\ObjectFactory.h>
#include <NiEngine/TilesetBlueprint.h>
#include <NiEngine/GameMode.h>
#include <NiEngine/ObjectTemplateBlueprint.h>
#include <NiEngine/ObjectBlueprint.h>

enum ObjectTypes
{
	None     = 0,
	Spike    = 1,
	Player   = 2,
	ExitDoor = 3,
	Ground   = 4,
	WalkerEnemy = 5,
	JumperEnemy = 6
};

class PlatformerObjectFactory : public ni::ObjectFactory
{
private:
	void SpawnObject      (ni::ObjectBlueprint object, ni::ObjectTemplateBlueprint& object_template, const std::vector<ni::TilesetBlueprint>& tileset_blueprints, ni::GameMode& mode, int type) override;
	void SpawnPlayer      (ni::ObjectBlueprint object, ni::ObjectTemplateBlueprint& object_template, std::string texture_key, sf::IntRect texture_coordinates, ni::GameMode& mode);
	void SpawnMovingObject(ni::ObjectBlueprint object, ni::ObjectTemplateBlueprint& object_template, std::string texture_key, sf::IntRect texture_coordinates, ni::GameMode& mode, std::string object_tag);
	void SpawnExitDoor    (ni::ObjectBlueprint object, ni::ObjectTemplateBlueprint& object_template, std::string texture_key, sf::IntRect texture_coordinates, ni::GameMode& mode);
	void SpawnWalkerEnemy (ni::ObjectBlueprint object, ni::ObjectTemplateBlueprint& object_template, std::string texture_key, sf::IntRect texture_coordinates, ni::GameMode& mode);
	void SpawnJumperEnemy (ni::ObjectBlueprint object, ni::ObjectTemplateBlueprint& object_template, std::string texture_key, sf::IntRect texture_coordinates, ni::GameMode& mode);

	template <typename T>
	T GetAttributeFromObject(ni::ObjectBlueprint& object, ni::ObjectTemplateBlueprint& object_template, const std::string& attribute)
	{
		T result = object_template.properties_map_.at(attribute).GetValue<T>();

		if (object.properties_.contains(attribute)) result = object.properties_.at(attribute).GetValue<T>();

		return result;
	}
};

