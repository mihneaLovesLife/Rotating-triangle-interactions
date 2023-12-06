/*
What is the purpose of this project?
I want to be able to handle physically accurate interactions of rotating triangles. (1)

*/

#include "TrianglePlate.h"
#include "Vector.h"
#include "Triangle.h"
#include "Segment.h"
#include "InputManager.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cassert>
#include <iomanip>

using namespace std;

void drawTrianglePlate(const TrianglePlate& plate, sf::RenderWindow& window)
{
	sf::VertexArray v(sf::LineStrip, 4);
	Vector a = plate.centerOfMass + plate.relativea.rotate(plate.angle);
	Vector b = plate.centerOfMass + plate.relativeb.rotate(plate.angle);
	Vector c = plate.centerOfMass + plate.relativec.rotate(plate.angle);
	v[0].position = sf::Vector2f(a.x, a.y);
	v[1].position = sf::Vector2f(b.x, b.y);
	v[2].position = sf::Vector2f(c.x, c.y);
	v[3] = v[0];
	window.draw(v);
}

double sqr(double a)
{
	return a * a;
}

void updateSystem(vector<TrianglePlate>& plates, const double dt, const double elasticity)
{
	for (auto& plate : plates)
	{
		plate.clearForces();
	}
	int n = (int)plates.size();
	vector<pair<Vector, double>> nw(n);
	for (int i = 0; i < n; i++)
	{
		TrianglePlate& plate = plates[i];
		Vector linearAcceleration = plate.linearForce / plate.mass;
		Vector newLinearVelocity = plate.linearVelocity + linearAcceleration * dt;

		double angularAcceleration = plate.torque / plate.momentOfInertia;
		double newAngularVelocity = plate.angularVelocity + angularAcceleration * dt;

		Vector centerOfMass = plate.centerOfMass + newLinearVelocity * dt;
		double angle = mod2PI(plate.angle + newAngularVelocity * dt);
		nw[i] = { centerOfMass, angle };
	}
	vector<Triangle> newTriangles(n);
	for (int i = 0; i < n; i++)
	{
		Vector a = nw[i].first + plates[i].relativea.rotate(nw[i].second);
		Vector b = nw[i].first + plates[i].relativeb.rotate(nw[i].second);
		Vector c = nw[i].first + plates[i].relativec.rotate(nw[i].second);
		newTriangles[i] = Triangle(a, b, c);
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			plates[i].checkFixed();
			plates[j].checkFixed();
			if (plates[i].fixed && plates[j].fixed)
			{
				continue;
			}
			auto [distance, _, point, normal] = getNearest(newTriangles[i], newTriangles[j]);
			if (distance == 0)
			{
				TrianglePlate& a = plates[i];
				TrianglePlate& b = plates[j];
				
				if (dot(a.velocityAtPoint(point) - b.velocityAtPoint(point), normal) < 0)
				{
					continue;
				}
				double impulseUp = -(1 + elasticity) * dot(a.velocityAtPoint(point) - b.velocityAtPoint(point), normal);
				double impulseDown = 0;
				impulseDown += dot(normal, normal) * ((double)1 / a.mass + (double)1 / b.mass);
				impulseDown += sqr(dot(normal, (point - a.centerOfMass).perpendicular())) / a.momentOfInertia;
				impulseDown += sqr(dot(normal, (point - b.centerOfMass).perpendicular())) / b.momentOfInertia;
				double impulse = impulseUp / impulseDown;

				a.linearVelocity += (impulse / a.mass) * normal;
				a.angularVelocity += (dot((point - a.centerOfMass), impulse * normal)) / a.momentOfInertia;

				b.linearVelocity += (-impulse / b.mass) * normal;
				b.angularVelocity += (dot((point - b.centerOfMass), -impulse * normal)) / b.momentOfInertia;
			}
		}
	}
	for (auto& plate : plates)
	{
		plate.update(dt);
	}
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(900, 900), "Rotating triangle interactions");

	sf::View view;
	view.setSize(sf::Vector2f(100, -100));
	view.setCenter(sf::Vector2f(50, 50));
	window.setView(view);

	sf::Clock frameClock;

	vector<TrianglePlate> plates;
	plates.push_back(TrianglePlate());
	plates.push_back(TrianglePlate());
	plates.push_back(TrianglePlate());
	plates.push_back(TrianglePlate());
	plates.push_back(TrianglePlate());
	plates.push_back(TrianglePlate());
	plates.push_back(TrianglePlate());
	plates.push_back(TrianglePlate());

	plates[0].setPlate(Vector(50 + 30, 50 + 10), Vector(20 + 33, 50 + 16), Vector(60 + 40, 20 + 10), 1);
	plates[1].setPlate(Vector(50 - 30, 50 - 10), Vector(50 - 33, 50 - 16), Vector(50 - 40, 50 - 10), 1);
	plates[2].setPlate(Vector(70 - 30, 50 - 10), Vector(70 - 33, 50 - 16), Vector(70 - 40, 50 - 10), 1);
	plates[3].setPlate(Vector(20 - 10, 20 - 10), Vector(20 - 10, 20 + 10), Vector(20 + 10, 20 - 10), 1);

	plates[4].setPlate(Vector(0, 0), Vector(0, 100), Vector(-50, 50), 1);
	plates[5].setPlate(Vector(0, 100), Vector(100, 100), Vector(50, 150), 1);
	plates[6].setPlate(Vector(100, 100), Vector(100, 0), Vector(150, 50), 1);
	plates[7].setPlate(Vector(100, 0), Vector(0, 0), Vector(50, -50), 1);
	plates[4].fixed = 1;
	plates[5].fixed = 1;
	plates[6].fixed = 1;
	plates[7].fixed = 1;

	{
		plates[0].linearVelocity = Vector(-1, 0) * 10;
		plates[1].linearVelocity = Vector(0, +1) * 5;
		plates[2].linearVelocity = Vector(-0.9, +0.8) * 7;
		plates[3].linearVelocity = Vector(+0.9, +0.8) * 7;

		plates[0].angularVelocity = 1;
		plates[1].angularVelocity = 0.5;
		plates[2].angularVelocity = 1;
		plates[3].angularVelocity = 1;

	}

	for (auto& plate : plates)
	{
		plate.mass = abs(cross(plate.relativeb - plate.relativea, plate.relativec - plate.relativea)) * 0.5 * 0.05;
	}

	sf::Clock fpsClock;
	int fps = 0;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}


		InputManager::update(window, {});


		float dt = frameClock.restart().asSeconds();
		fps++;

		if (fpsClock.getElapsedTime().asSeconds() >= 1)
		{
			cout << "fps = " << fps << "\n";
			fpsClock.restart();
			fps = 0;
		}

		window.clear();
		updateSystem(plates, dt, 0);

		for (auto& plate : plates)
		{
			drawTrianglePlate(plate, window);
		}

		window.display();
	}

	return 0;
}
