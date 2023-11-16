#pragma once

#include "Vector.h"

class Segment
{
public:
	Vector a;
	Vector b;

	Segment();
	Segment(Vector a, Vector b);

	Vector nearestPoint(Vector point);
	double distanceToPoint(Vector point);
};

bool doIntersect(Segment segment1, Segment segment2);