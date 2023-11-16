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
};