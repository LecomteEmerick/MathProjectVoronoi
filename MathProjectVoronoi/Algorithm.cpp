#include "Algorithm.h"

void getBottomLeftPoint(std::vector<Point2D> &pointCloud, Point2D &outPoint)
{
	if (pointCloud.size() < 1)
		return;

	outPoint = pointCloud[0];
	for (int i = 1; i < pointCloud.size(); ++i)
	{
		if (outPoint->y <= pointCloud[i].y)
		{
			if (outPoint->y < pointCloud[i].y)
			{
				outPoint = &pointCloud[i];
			}
			else 
			{
				if (outPoint->x > pointCloud[i].x)
				{
					outPoint = &pointCloud[i];
				}
			}
		}
	}
}

void MarcheDeJarvis(PointPool pointPool)
{
	Point2D point();
	getBottomLeftPoint(pointPool.cloudPoint, &point);
}
