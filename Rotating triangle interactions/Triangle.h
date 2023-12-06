#pragma once

#include "Vector.h"
#include "Segment.h"
#include <utility>
#include <array>
#include <optional>


// class ContactInfo

class Triangle
{
public:
	static std::pair<Vector, Vector> thePair;
	static bool tag;
	std::pair<double, Segment> getNearest(Triangle triangle1, Triangle triangle2);

	Vector a;
	Vector b;
	Vector c;

	Triangle();
	Triangle(Vector a, Vector b, Vector c);
	bool isCounterClockwise();

	
	std::array<Vector, 3> vertices();
	std::array<Segment, 3> segments();
};

bool isInside(Triangle triangle, Vector point);
std::optional<Vector> getIntersection(Triangle triangle1, Triangle triangle2);
std::pair<double, Segment> getNearest(Triangle triangle1, Triangle triangle2);