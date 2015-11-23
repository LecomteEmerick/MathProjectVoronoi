#pragma once
#include "Point2D.h"
#include "DrawableItem.h"
#include "Color.h"

#include "gtc\matrix_transform.hpp"

#include <vector>

class PointPool : public DrawableItem
{
public:
							PointPool();
	void					Initialize();
	void					AddPoint(double x, double y, const glm::mat4& cameraMatrix, const glm::vec4& viewport);
	void					Draw(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix, const glm::mat4& worldTransform, const float& time) const;
	void					Destroy();
	std::vector<Point2D>	cloudPoint;
							~PointPool();
private:
	Color					CurrentColor;

	void					ConstructEBO();
	void					ConstructVBO();
};

