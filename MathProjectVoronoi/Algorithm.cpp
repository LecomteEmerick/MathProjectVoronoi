#include "Algorithm.h"
#include <memory>

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

int GetLeftNearestPointIndex(std::vector<Point2D> &pointCloud,int RefIndexPoint)
{
	float CurrentXDistance = -1;
	Point2D tmp;
	Point2D RefPoint = pointCloud[RefIndexPoint];
	for (int i = 0; i < pointCloud.size(); ++i)
	{
		if (i != RefIndexPoint)
		{
			tmp = pointCloud[i];
			float xDistance = sqrtf((tmp.x - RefPoint.x) * (tmp.x - RefPoint.x));
			if (xDistance < CurrentXDistance)
			{

			}
		}
	}
}

void Algorithm::MarcheDeJarvis(PointPool &pointPool)
{
	std::vector<Point2D> env;

	int currentIndex = getBottomLeftPointIndex(pointPool.cloudPoint);
	if (currentIndex == -1)
		return;

	env.push_back(pointPool.cloudPoint[currentIndex]);


}
