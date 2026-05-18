#pragma once

#include <string>

namespace ni {

struct Animation
{
	std::string key_  = "";
	int frame_count   = 0;
	int start_frame   = 0;
	int animation_row = 0;

	int GetEndFrame() const
	{
		return start_frame + frame_count;
	}
};

}