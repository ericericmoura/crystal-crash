#pragma once

#include <string>
#include <regex>

namespace ni {

struct FileUtility
{	
	static inline std::string RemoveRelativePaths(const std::string& path)
	{
		return std::regex_replace(path, std::regex(R"(\.\./)"), "");
	}
};

}