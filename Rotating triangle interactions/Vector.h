#pragma once

class Vector
{
public:
	double x;
	double y;

	Vector(double x, double y);
	Vector();

	Vector rotateAround(const Vector& origin, const double angle) const;
	Vector rotate(const double angle) const;
	Vector perpendicular() const;

	Vector operator + (const Vector& rhs) const;
	Vector operator - (const Vector& rhs) const;
	Vector operator * (const double scalar) const;
	Vector operator / (const double scalar) const;

	Vector& operator += (const Vector& rhs);
};

double dot(Vector a, Vector b);
double distanceSquared(Vector a, Vector b);