#pragma once
#include "Color.h"
#include <cmath>

class Point2D
{
public:
	float			x, y;
	float			z = 1.0;
	Color			color;

					Point2D() : x(0), y(0), color(0,0,0) {}
					Point2D(float x, float y, Color c) : x(x), y(y), color(c) {}
					Point2D(float x, float y) : x(x), y(y), color(0.0,0.0,0.0) {}
					~Point2D() {}

	float			Point2D::Distance(Point2D point) { return std::sqrt((point.x - this->x) * (point.x - this->x) + (point.y - this->y) * (point.y - this->y)); }
};

