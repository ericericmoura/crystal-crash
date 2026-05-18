#pragma once

namespace ni {

struct MathUtility
{
	template <typename T>
	static int GetSign(T number)
	{
		return (number > 0) - (number < 0);
	}

	static float EaseIn(float t) { return t * t; }

	static float EaseOut(float t) { return t * (2.0f - t); }

	static float EaseInOut(float t) { return t * t * (3.0f - 2.0f * t); }
};

}