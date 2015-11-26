#include "Algorithm.h"
#include <memory>
#include <math.h>

int getBottomLeftPointIndex(std::vector<Point2D> &pointCloud)
{
	if (pointCloud.size() < 1)
		return -1;

	int index = 0;
	for (int i = 1; i < pointCloud.size(); ++i)
	{
		if (pointCloud[index].y >= pointCloud[i].y)
		{
			if (pointCloud[index].y > pointCloud[i].y)
			{
				index = i;
			}
			else 
			{
				if (pointCloud[index].x > pointCloud[i].x)
				{
					index = i;
				}
			}
		}
	}
	return index;
}

float Angle(Point2D ref, Point2D point)
{
	Point2D projection(point.x, ref.y);

	float distanceHypo = ref.Distance(point);
	float distanceAdjacent = ref.Distance(projection);

	float angle = acosf(distanceHypo / distanceAdjacent);

	return acosf(distanceHypo / distanceAdjacent);
}

void Algorithm::MarcheDeJarvis(PointPool &pointPool)
{
	std::vector<Point2D> env;
	int savedIndex, firstIndex, currentIndex = 0;
	firstIndex = getBottomLeftPointIndex(pointPool.cloudPoint);
	if (firstIndex == -1)
		return;

	currentIndex = firstIndex;
	float minAngle = 0, previousAngle, tmpAngle = 0;

	Point2D previousPoint, currentPoint;

	do
	{
		previousPoint = pointPool.cloudPoint[currentIndex];
		previousAngle = minAngle;
		minAngle = 360;
		env.push_back(previousPoint);
		for (int i = 0; i < pointPool.cloudPoint.size(); ++i)
		{
			if (i == currentIndex)
			{
				continue;
			}
			//
			currentPoint = pointPool.cloudPoint[i];
			tmpAngle = Angle(previousPoint, currentPoint);
			//
			if (tmpAngle - previousAngle < minAngle)
			{
				minAngle = tmpAngle - previousAngle;
				savedIndex = i;
			}
			else if (tmpAngle - previousAngle == minAngle)
			{
				if (previousPoint.Distance(currentPoint) < previousPoint.Distance(pointPool.cloudPoint[savedIndex]))
					savedIndex = i;
			}
		}

	}while(currentIndex != firstIndex);
}
