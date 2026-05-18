#include <NiEngine/ObjectFactory.h>

#include <format>
#include <variant>
#include <string>
#include <vector>

#include <NiEngine/FileUtility.h>
#include <NiEngine/Level.h>
#include <NiEngine/ObjectTemplateBlueprint.h>
#include <NiEngine/ObjectLayerBlueprint.h>
#include <NiEngine/TilesetBlueprint.h>
#include <NiEngine/DataHandler.h>

void ni::ObjectFactory::LoadObjects(ObjectLayerBlueprint & layer_blueprint, const std::vector<TilesetBlueprint>& tileset_blueprints, GameMode & mode)
{
	for (auto& object : layer_blueprint.objects_)
	{
		std::string template_path = FileUtility::RemoveRelativePaths(object.template_filepath_);
		template_path = std::format("{}/{}", Level::kDefaultLevelsDirectory, template_path);

		auto it = template_map_.find(template_path);

		if (it == template_map_.end())
		{
			DataHandler<ObjectTemplateBlueprint> template_data_handler(template_path);
			template_map_[template_path] = template_data_handler.GetBlueprint();
		}

		ObjectTemplateBlueprint& template_blueprint = template_map_[template_path];

		auto& value = template_blueprint.properties_map_.at("type_id").value_;
		if (std::holds_alternative<int>(value))
		{
			int type = std::get<int>(value);

			SpawnObject(object, template_blueprint, tileset_blueprints, mode, type);
		}
	}
}