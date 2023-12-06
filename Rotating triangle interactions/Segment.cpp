#include "Segment.h"
#include <cassert>
#include <algorithm>

using namespace std;

Segment::Segment()
{
}

Segment::Segment(Vector a, Vector b) :
	a(a),
	b(b)
{

}

Vector Segment::nearestPoint(Vector p)
{
	if (a.x == b.x && a.y == b.y)
	{
		return a;
	}
	/*
	
	a --- b

		p

	d = b - a
	d2 = d.perp()

	p + d2 * ?? = a + q * (b - a)
	p.x * d2.y + d2.x * d2.y ?? = a.x * d2.y + q * d.x * d2.y
	p.y * d2.x + d2.y * d2.x * ?? = a.y * d2.x + q * d.y * d2.x

	p.x * d2.y - p.y * d2.x = a.x * d2.y - a.y * d2.x + q * (d.x * d2.y - d.y * d2.x)
	p X d2 = a X d2 + q * (D X d2)
	(p - a) X d2 = q * (D X d2)

	q = (p - a) * (b - a) / ((b - a) * (b - a))
	
	*/
	double q = min((double)1, max((double)0, dot(b - a, p - a) / dot(b - a, b - a)));
	return a + q * (b - a);
}


double Segment::distanceToPoint(Vector point)
{
	return getDistance(point, nearestPoint(point));
}

int getSign(double x)
{
	if (abs(x) < 1e-10)
	{
		return 0;
	}
	if (x > 0)
	{
		return +1;
	}
	return -1;
}

bool isOnSegment(Vector a, Vector b, Vector p)
{
	return min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x) && min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y);
}

bool doIntersect(Segment s1, Segment s2)
{
	Vector p1 = s1.a;
	Vector p2 = s1.b;

	Vector p3 = s2.a;
	Vector p4 = s2.b;

	int dir1 = getSign(getSignedDoubleArea(p3, p4, p1));
	int dir2 = getSign(getSignedDoubleArea(p3, p4, p2));
	int dir3 = getSign(getSignedDoubleArea(p1, p2, p3));
	int dir4 = getSign(getSignedDoubleArea(p1, p2, p4));

	if (dir1 * dir2 == -1 && dir3 * dir4 == -1)
	{
		return true;
	}

	if (dir1 == 0 && isOnSegment(p3, p4, p1))
	{
		return true;
	}

	if (dir2 == 0 && isOnSegment(p3, p4, p2))
	{
		return true;
	}

	if (dir3 == 0 && isOnSegment(p1, p2, p3))
	{
		return true;
	}
	
	if (dir4 == 0 && isOnSegment(p1, p2, p4))
	{
		return true;
	}

	return false;
}

Vector getIntersection(Segment segment1, Segment segment2)
{
	assert(doIntersect(segment1, segment2));

	Vector A1 = segment1.a;
	Vector B1 = segment1.b;

	Vector A2 = segment2.a;
	Vector B2 = segment2.b;

	return ((B1 - A1) * (cross(A2 - A1, B2 - A2) / cross(B1 - A1, B2 - A2))) + A1;
}