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
	return (Vector(x, y) - origin).rotate(angle) + origin;
}

Vector Vector::rotate(const double angle) const
{
	double Sin = sin(angle);
	double Cos = cos(angle);
	double nx = x * Cos - y * Sin;
	double ny = x * Sin + y * Cos;
	return Vector(nx, ny);
}

Vector Vector::normalize() const
{
	return *this / norm();
}

double Vector::normSquared() const
{
	return dot(*this, *this);
}

double Vector::norm() const
{
	return sqrt(normSquared());
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
	return dot(a - b, a - b);
}

double cross(Vector a, Vector b)
{
	return dot(a.perpendicular(), b);
}