#pragma once

#include "Vector.h"
#include "Triangle.h"

class TrianglePlate
{
public:
	Vector relativea;
	Vector relativeb;
	Vector relativec;
	Vector centerOfMass;

	Vector linearVelocity;
	double angularVelocity;
	double angle;
	Vector linearForce;
	double torque;
	double momentOfInertia;

	double mass;
	bool fixed;

	TrianglePlate();
	
	void update(double dt);
	void setPlate(Vector a, Vector b, Vector c, double m);

	void applyForce(Vector position, Vector force);
	void clearForces();

	Triangle getTriangle();

	Vector velocityAtPoint(Vector point);

	void checkFixed();
};


double mod2PI(double angle);