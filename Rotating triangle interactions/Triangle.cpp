#include "Triangle.h"
#include "Vector"
#include "Segment.h"
#include <algorithm>
#include <limits>

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

bool isInside(Triangle triangle, Vector point)
{
	Vector a = triangle.a;
	Vector b = triangle.b;
	Vector c = triangle.c;
	
	double area = getArea(a, b, c);
	double area2 = getArea(a, b, point) + getArea(b, c, point) + getArea(c, a, point);

	return abs(area - area2) < 1e-10;
}

std::pair<bool, Vector> getIntersection(Triangle triangle1, Triangle triangle2)
{
	Vector a1 = triangle1.a;
	Vector b1 = triangle1.b;
	Vector c1 = triangle1.c;

	Vector a2 = triangle2.a;
	Vector b2 = triangle2.b;
	Vector c2 = triangle2.c;

	if (isInside(triangle2, a1)) return { true, a1 };
	if (isInside(triangle2, b1)) return { true, b1 };
	if (isInside(triangle2, c1)) return { true, c1 };

	if (isInside(triangle1, a2)) return { true, a2 };
	if (isInside(triangle1, b2)) return { true, b2 };
	if (isInside(triangle1, c2)) return { true, c2 };

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
				return { true, getIntersection(s0[i], s1[j]) };
			}
		}
	}

	return { false, Vector() };
}


pair<double, pair<Vector, Vector>> getNearest(Triangle triangle1, Triangle triangle2)
{
	auto it = getIntersection(triangle1, triangle2);
	if (it.first)
	{
		return { true, {it.second, it.second} };
	}

	pair<double, pair<Vector, Vector>> sol;
	sol.first = std::numeric_limits<double>::infinity();

	Vector v1[3];
	Vector v2[3];

	v1[0] = triangle1.a;
	v1[1] = triangle1.b;
	v1[2] = triangle1.c;

	v2[0] = triangle2.a;
	v2[1] = triangle2.b;
	v2[2] = triangle2.c;
	
	for (int iter = 0; iter <= 1; iter++)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				int k = (j + 1) % 3;
				Segment segment(v2[j], v2[k]);
				pair<double, pair<Vector, Vector>> now;
				Vector point = segment.nearestPoint(v1[i]);
				now.first = distanceSquared(point, v1[i]);
				if (now.first >= sol.first)
				{
					continue;
				}
				now.second.first = v1[i];
				now.second.second = point;
				if (iter == 1)
				{
					swap(now.second.first, now.second.second);
				}
				sol = now;
			}
		}
		for (int i = 0; i < 3; i++)
		{
			swap(v1[i], v2[i]);
		}
	}
	sol.first = sqrt(sol.first);
	return sol;
}