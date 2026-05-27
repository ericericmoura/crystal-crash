#pragma once

#include <collision.h>
#include <math_functions.h>

#include <vector>

#include <SFML/System/Vector2.hpp>

#include "PolygonBlueprint.h"
#include "Converter.h"

namespace ni 
{

struct PolygonUtility
{
	static b2Polygon CreatePolygonFromOffsetPoints(PolygonBlueprint polygon_blueprint, sf::Vector2i polygon_size)
	{
		const int kVerticesCount = polygon_blueprint.offset_points_.size();

		sf::Vector2i initial_vertice = polygon_blueprint.position_;

		sf::Vector2i half_size = { polygon_size.x / 2, polygon_size.y / 2 };

		std::vector<b2Vec2> points = {};
		for (int i = 0; i < kVerticesCount; ++i)
		{
			auto offset = polygon_blueprint.offset_points_.at(i);
			auto vertice_position = initial_vertice + offset;

			vertice_position -= half_size;

			points.push_back(ni::Converter::PixelsToMeters(vertice_position));
		}

		b2Hull hull = b2ComputeHull(points.data(), kVerticesCount);
		return b2MakePolygon(&hull, 0);
	}
};

}