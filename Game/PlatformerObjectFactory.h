#pragma once

#include <id.h>

#include <vector>
#include <string>

#include <NiEngine/ObjectFactory.h>
#include <NiEngine/TilesetBlueprint.h>
#include <NiEngine/GameMode.h>
#include <NiEngine/ObjectTemplateBlueprint.h>
#include <NiEngine/ObjectBlueprint.h>

enum ObjectTypes
{
	None      = 0,
	Slingshot = 1,
	Ammo      = 2
};

class PlatformerObjectFactory : public ni::ObjectFactory
{
public:
	void SetWorldId(b2WorldId world_id) { world_id_ = world_id; }

private:
	b2WorldId world_id_;

	void SpawnObject   (ni::ObjectBlueprint object, ni::ObjectTemplateBlueprint& object_template, const std::vector<ni::TilesetBlueprint>& tileset_blueprints, ni::GameMode& mode, int type) override;
	void SpawnSlingshot(ni::ObjectBlueprint object, ni::ObjectTemplateBlueprint& object_template, ni::TilesetBlueprint& tileset, ni::GameMode& mode);
	void SpawnAmmo     (ni::ObjectBlueprint object, ni::ObjectTemplateBlueprint& object_template, ni::TilesetBlueprint& tileset, ni::GameMode& mode);

	template <typename T>
	T GetAttributeFromObject(ni::ObjectBlueprint& object, ni::ObjectTemplateBlueprint& object_template, const std::string& attribute)
	{
		auto it = object_template.properties_map_.find(attribute);

		if (it == object_template.properties_map_.end())
		{
			return {};
		}

		T result = object_template.properties_map_.at(attribute).GetValue<T>();

		if (object.properties_.contains(attribute)) result = object.properties_.at(attribute).GetValue<T>();

		return result;
	}
};

