#pragma once

#include <iostream>
#include <string>
#include <variant>
#include <utility>
#include <vector>
#include <array>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace ni {

using PropertyValue = std::variant<std::string, float, int, bool, std::vector<int>>;

struct PropertyBlueprint
{
	std::string   name_  = "";
	std::string   type_  = "";
	PropertyValue value_ = {};

	template<typename T>
	T GetValue()
	{
		if (std::holds_alternative<T>(value_))
		{ 
			return std::get<T>(value_);
		}
		return T{};
	}

	template<typename T>
	bool HasValue()
	{
		return std::holds_alternative<T>(value_);
	}
};

inline void to_json(json& j, const PropertyBlueprint& pb)
{
	j =
	{
		{ "name",  pb.name_  },
		{ "type",  pb.type_  }		
	};

	std::visit([&j](const auto& val)
	{
		j["value"] = val;
	}, pb.value_);
}

inline void from_json(const json& j, PropertyBlueprint& pb)
{
	j.at("name") .get_to(pb.name_);
	j.at("type") .get_to(pb.type_);
	
	if      (pb.type_ == "float" ) pb.value_ = j.at("value").get<float>();
	else if (pb.type_ == "int"   ) pb.value_ = j.at("value").get<int>  ();
	else if (pb.type_ == "bool"  ) pb.value_ = j.at("value").get<bool> ();	
	else if (pb.type_ == "string") pb.value_ = j.at("value").get<std::string>();
	else if (pb.type_ == "list")
	{
		if (j.at("value").size() == 0)
		{
			return;
		}
		json first_value = j.at("value").front();

		if (first_value.at("type") == "int")
		{
			pb.value_ = std::vector<int>();
			for (json item : j.at("value"))
			{
				std::get<std::vector<int>>(pb.value_).push_back(item.at("value").get<int>());
			}
		}
	}
}

}