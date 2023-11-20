#pragma once

#include "Vector.h"
#include <utility>

class Triangle
{
public:
	Vector a;
	Vector b;
	Vector c;

	Triangle();
	Triangle(Vector a, Vector b, Vector c);
};

bool isInside(Triangle triangle, Vector point);
std::pair<bool, Vector> getIntersection(Triangle triangle1, Triangle triangle2);
std::pair<double, std::pair<Vector, Vector>> getNearest(Triangle triangle1, Triangle triangle2);
