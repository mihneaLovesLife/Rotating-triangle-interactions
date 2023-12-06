#include "TrianglePlate.h"
#include <cmath>
#include <limits>
#include "Segment.h"

const double PI = 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651328230664709384460955058223172535940812848111745028410270193852110555964462294895493038196442881097566593344612847564823378678316527120190914564856692346034861045432664821339360726024914127372458700660631558817488152092096282925409171536436789259036001133053054882046652138414695194151160943305727036575959195309218611738193261179310511854807446237996274956735188575272489122793818301194912983367336244065664308602139494639522473719070217986094370277053;

TrianglePlate::TrianglePlate() :
	relativea(Vector(0, 0)),
	relativeb(Vector(0, 0)),
	relativec(Vector(0, 0)),
	centerOfMass(Vector(0, 0)),
	linearVelocity(Vector(0, 0)),
	angularVelocity(0),
	angle(0),
	linearForce(Vector(0, 0)),
	torque(0),
	mass(0),
	momentOfInertia(0),
	fixed(false)
{

}

void TrianglePlate::checkFixed()
{
	if (fixed)
	{
		mass = std::numeric_limits<double>::infinity();
		mass = 1;
		linearVelocity = Vector(0, 0);
		angularVelocity = 0;
		angle = 0;
	}

}

double mod2PI(double angle)
{
	angle = fmod(angle, 2 * PI);
	angle += 2 * PI * (angle < 0);
	return angle;
}

Vector TrianglePlate::velocityAtPoint(Vector point)
{
	return (angularVelocity * (point - centerOfMass).perpendicular()) + linearVelocity;
}

void TrianglePlate::update(double dt)
{
	if (fixed)
	{
		return;
	}
	Vector linearAcceleration = linearForce / mass;
	linearVelocity += linearAcceleration * dt;

	double angularAcceleration = torque / momentOfInertia;
	angularVelocity += angularAcceleration * dt;

	centerOfMass += linearVelocity * dt;
	angle = mod2PI(angle + angularVelocity * dt);
}

void TrianglePlate::setPlate(Vector a, Vector b, Vector c, double m)
{
	centerOfMass = (a + b + c) / 3;
	relativea = a - centerOfMass;
	relativeb = b - centerOfMass;
	relativec = c - centerOfMass;
	mass = m;
	momentOfInertia = mass / (double)1 * (distanceSquared(relativea, relativeb) + distanceSquared(relativea, relativec) + distanceSquared(relativeb, relativec));
}

void TrianglePlate::applyForce(Vector position, Vector force)
{
	linearForce += force;
	torque += cross(position - centerOfMass, force);
}

void TrianglePlate::clearForces()
{
	linearForce = Vector(0, 0);
	torque = 0;
}

Triangle TrianglePlate::getTriangle()
{
	return Triangle(centerOfMass + relativea.rotate(angle), centerOfMass + relativeb.rotate(angle), centerOfMass + relativec.rotate(angle));
}
