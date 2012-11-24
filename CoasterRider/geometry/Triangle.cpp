#include "Triangle.h"

/**#############################################################################
	intersects() - does this triangle intersect with another?
		IN:
			t2 - the other triangle.
		OUT:
			(return value) - true if this and the other triangles intersect.
#############################################################################**/

bool Triangle::intersects(Triangle t2)
	{
	vector3df outIntersection;
	if(getIntersectionWithLimitedLine (line3df(t2.pointA,t2.pointB),outIntersection))
		return true;
	if(getIntersectionWithLimitedLine (line3df(t2.pointA,t2.pointB),outIntersection))
		return true;
	if(getIntersectionWithLimitedLine (line3df(t2.pointA,t2.pointB),outIntersection))
		return true;
	#if 0
	if(isPointInside(t2.pointA))
		return true;
	if(isPointInside(t2.pointB))
		return true;
	if(isPointInside(t2.pointC))
		return true;
	#endif
	return false;
	}
