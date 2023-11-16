#include "Segment.h"

Segment::Segment()
{
}

Segment::Segment(Vector a, Vector b) :
	a(a),
	b(b)
{

}

#include <iostream>

using namespace std;

Vector Segment::nearestPoint(Vector p)
{
	if (a.x == b.x && a.y == b.y)
	{
		return a;
	}
	double q = min((double)1, max((double)0, dot(b - a, p - a) / dot(b - a, b - a)));
	return a + q * (b - a);
}
