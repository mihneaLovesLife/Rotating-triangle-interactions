#include <cassert>
#include "Triangle.h"
#include "Vector"
#include "Segment.h"
#include <algorithm>
#include <limits>
#include <optional>
#include <ranges>

using namespace std;


Triangle::Triangle()
{

}

Triangle::Triangle(Vector a, Vector b, Vector c) :
	a(a),
	b(b),
	c(c)
{

}

bool Triangle::isCounterClockwise()
{
	return cross(b - a, c - a) > 0;
}

std::array<Vector, 3> Triangle::vertices()
{
	return std::array<Vector, 3> { a, b, c };
}

std::array<Segment, 3> Triangle::segments()
{
	return std::array<Segment, 3> { Segment(a, b), Segment(b, c), Segment(c, a) };
}

bool isInside(Triangle triangle, Vector point)
{
	Vector a = triangle.a;
	Vector b = triangle.b;
	Vector c = triangle.c;

	double area = getArea(a, b, c);
	double area2 = getArea(a, b, point) + getArea(b, c, point) + getArea(c, a, point);

	return abs(area - area2) < 1e-10;
}

std::optional<Vector> getIntersection(Triangle triangle1, Triangle triangle2)
{
	Vector a1 = triangle1.a;
	Vector b1 = triangle1.b;
	Vector c1 = triangle1.c;

	Vector a2 = triangle2.a;
	Vector b2 = triangle2.b;
	Vector c2 = triangle2.c;

	if (isInside(triangle2, a1)) return a1;
	if (isInside(triangle2, b1)) return b1;
	if (isInside(triangle2, c1)) return c1;

	if (isInside(triangle1, a2)) return a2;
	if (isInside(triangle1, b2)) return b2;
	if (isInside(triangle1, c2)) return c2;

	Segment s0[3];
	Segment s1[3];

	s0[0] = Segment(a1, b1);
	s0[1] = Segment(b1, c1);
	s0[2] = Segment(c1, a1);

	s1[0] = Segment(a2, b2);
	s1[1] = Segment(b2, c2);
	s1[2] = Segment(c2, a2);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (doIntersect(s0[i], s1[j]))
			{
				return getIntersection(s0[i], s1[j]);
			}
		}
	}

	return {};
}

int side(Vector p1, Vector p2, Vector a)
{
	return cross(p2 - p1, a - p1) > 0;
}

ContactInfo getNearest(Triangle triangle1, Triangle triangle2)
{
	auto intersection = getIntersection(triangle1, triangle2);
	double optimal = std::numeric_limits<double>::infinity();
	ContactInfo solution;
	solution.distance = std::numeric_limits<double>::infinity();
	for (auto& point : triangle1.vertices())
	{
		for (auto& segment : triangle2.segments())
		{
			Vector nearestPoint = segment.nearestPoint(point);
			double theDistance = getDistance(nearestPoint, point);
			if (theDistance < optimal)
			{
				optimal = theDistance;
				Vector segmentNorm = (segment.b - segment.a).perpendicular().normalize();
				if (!triangle2.isCounterClockwise()) segmentNorm = segmentNorm * -1;
				solution = { theDistance, {point, nearestPoint }, nearestPoint, segmentNorm };
			}
		}
	}
	for (auto& point : triangle2.vertices())
	{
		for (auto& segment : triangle1.segments())
		{
			Vector nearestPoint = segment.nearestPoint(point);
			double theDistance = getDistance(nearestPoint, point);
			if (theDistance < optimal)
			{
				optimal = theDistance;
				Vector segmentNorm = (segment.b - segment.a).perpendicular().normalize();
				if (!triangle1.isCounterClockwise()) segmentNorm = segmentNorm * -1;
				solution = { theDistance, {nearestPoint, point }, nearestPoint, -1 * segmentNorm };
			}
		}
	}
	if (intersection)
	{
		return { 0, Segment(*intersection, *intersection), solution.hitPoint, solution.normal };
	}
	return solution;
}
