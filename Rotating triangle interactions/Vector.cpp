#include "Vector.h"
#include <cmath>

Vector::Vector(double x, double y) : 
	x(x), 
	y(y) 
{
}

Vector::Vector() :
	x(0),
	y(0)
{
}

Vector Vector::rotateAround(const Vector& origin, const double angle) const
{
	double Sin = sin(angle);
	double Cos = cos(angle);
	double rx = x - origin.x;
	double ry = y - origin.y;
	double nx = rx * Cos - ry * Sin;
	double ny = rx * Sin + ry * Cos;
	return Vector(nx + origin.x, ny + origin.y);
}

Vector Vector::rotate(const double angle) const
{
	double Sin = sin(angle);
	double Cos = cos(angle);
	double nx = x * Cos - y * Sin;
	double ny = x * Sin + y * Cos;
	return Vector(nx, ny);
}


Vector Vector::operator * (const double scalar) const
{
	return Vector(x * scalar, y * scalar);
}

Vector Vector::operator / (const double scalar) const
{
	return Vector(x / scalar, y / scalar);
}

Vector Vector::operator + (const Vector& rhs) const
{
	return Vector(x + rhs.x, y + rhs.y);
}

Vector Vector::operator - (const Vector& rhs) const
{
	return Vector(x - rhs.x, y - rhs.y);
}

Vector& Vector::operator += (const Vector& rhs)
{
	x += rhs.x;
	y += rhs.y;
	return *this;
}

Vector Vector::perpendicular() const
{
	return Vector(-y, x);
}

double dot(Vector a, Vector b)
{
	return a.x * b.x + a.y * b.y;
}

double distanceSquared(Vector a, Vector b)
{
	return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}