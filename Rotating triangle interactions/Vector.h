#pragma once

class Vector
{
public:
	double x;
	double y;

	Vector();
	Vector(double x, double y);

	Vector rotate(const double angle) const;
	Vector rotateAround(const Vector& origin, const double angle) const;
	Vector perpendicular() const;
	double norm() const;
	double normSquared() const;	
	Vector normalize() const;

	Vector operator + (const Vector& rhs) const;
	Vector operator - (const Vector& rhs) const;
	Vector operator * (const double scalar) const;
	Vector operator / (const double scalar) const;

	Vector& operator += (const Vector& rhs);
};

Vector operator * (const double scalar, const Vector& v);
double dot(Vector a, Vector b);
double distanceSquared(Vector a, Vector b);
double getDistance(Vector a, Vector b);
double cross(Vector a, Vector b);
double getSignedDoubleArea(Vector a, Vector b, Vector c);
double getArea(Vector a, Vector b, Vector c);