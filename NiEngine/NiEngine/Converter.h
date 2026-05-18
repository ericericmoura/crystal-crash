#pragma once

#include <math_functions.h>

#include <numbers>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Angle.hpp>

namespace ni
{

struct Converter
{
	static inline float pixels_per_meters_ = 32.0;

	template<typename T>
	static T PixelsToMeters(const T& x) { return x / pixels_per_meters_; };

	template<typename T>
	static T MetersToPixels(const T& x) { return x * pixels_per_meters_; };

	template<typename T>
	static T DegToRad(const T& x) { return std::numbers::pi * x / 180.0; };

	template<typename T>
	static T RadToDeg(const T& x) { return 180.0 * x / std::numbers::pi; }

	inline static sf::Vector2f MetersToPixels(const b2Vec2& v)
	{
		return sf::Vector2f(v.x * pixels_per_meters_, v.y * pixels_per_meters_);
	}

	inline static b2Vec2 PixelsToMeters(const sf::Vector2f& v)
	{
		return b2Vec2(v.x / pixels_per_meters_, v.y / pixels_per_meters_);
	}

	inline static b2Vec2 PixelsToMeters(const sf::Vector2i& v)
	{
		return b2Vec2(v.x / pixels_per_meters_, v.y / pixels_per_meters_);
	}

	inline static sf::Angle RotToAngle(const b2Rot& v)
	{
		return sf::Angle(sf::radians(b2Rot_GetAngle(v)));
	}
};

}
