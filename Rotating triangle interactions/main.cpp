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
	//cout << fixed << setprecision(6) << "(" << a.x << ", " << a.y << "); (" << b.x << ", " << b.y << "); (" << c.x << ", " << c.y << ")\n";
	v[0].position = sf::Vector2f(a.x, a.y);
	v[1].position = sf::Vector2f(b.x, b.y);
	v[2].position = sf::Vector2f(c.x, c.y);
	v[3] = v[0];

	window.draw(v);
}

bool killTime;

double sqr(double a)
{
	return a * a;
}

int cnt = 0;

void drawPoint(sf::RenderWindow& window, Vector point, sf::Color color)
{
	sf::CircleShape shape;
	shape.setRadius(0.5f);
	shape.setOrigin(shape.getRadius() * sf::Vector2f(1, 1));
	shape.setPosition(sf::Vector2f(point.x, point.y));
	shape.setFillColor(color);
	window.draw(shape);
}

void drawLine(sf::RenderWindow& window, Vector point1, Vector point2, sf::Color color)
{
	sf::VertexArray varray(sf::LinesStrip, 2);
	varray[0].position = sf::Vector2f(point1.x, point1.y);
	varray[1].position = sf::Vector2f(point2.x, point2.y);
	varray[0].color = varray[1].color = color;
	window.draw(varray);
}

int icr = 0;

void updateSystem(vector<TrianglePlate>& plates, double dt, sf::RenderWindow& window)
{
	if (killTime)
	{
		return;
	}
	if (dt < 1e-10)
	{
		return;
	}
	for (auto& plate : plates)
	{
		plate.clearForces();
	}
	int n = (int)plates.size();
	{
		plates[0].applyForce(plates[0].centerOfMass + Vector(0, 1) * 0.1, Vector(-1, 0) * 20000);
		plates[0].applyForce(plates[0].centerOfMass - Vector(0, 1) * 0.1, Vector(+1, 0) * 10000);
	}
	{
		plates[1].applyForce(plates[1].centerOfMass, Vector(0, +1) * 40);
	}
	{
		plates[2].applyForce(plates[2].centerOfMass, Vector(-1, +0.5) * 40);
	}
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
	//for (int i = 0; i < n; i++)
	//{
	//	for (int j = i + 1; j < n; j++)
	//	{
	//		auto it = getNearest(newTriangles[i], newTriangles[j]);
	//		if (it.first < 1e-10)
	//		{

	//		}
	//	}
	//}
	//for (int i = 0; i < n; i++)
	//{
	//	for (int j = i + 1; j < n; j++)
	//	{
	//		auto it = getNearest(newTriangles[i], newTriangles[j]);

	//		if (Triangle::tag)
	//		{
	//			//cout << "here, dividing, yay! : " << fixed << setprecision(10) << it.first << " " << dt << "\n";
	//			updateSystem(plates, dt * 0.5);

	//			if (killTime)
	//			{
	//				return;
	//			}
	//			updateSystem(plates, dt * 0.5);

	//			if (killTime)
	//			{
	//				return;
	//			}
	//			return;
	//		}
	//	}
	//}
	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < n; j++)
		//for (int j = i + 1; j < n; j++)
		{
			auto it = getNearest(newTriangles[i], newTriangles[j]);
			if (Triangle::tag)
			{
				TrianglePlate& a = plates[i];
				TrianglePlate& b = plates[j];
				const double elasticity = 1;

				Vector point = Triangle::thePair.first;
				Vector normal = Triangle::thePair.second;

				drawPoint(window, point, sf::Color::Green);
				
				drawLine(window, point, a.velocityAtPoint(point) * 0.1 + point, sf::Color::Yellow);
				drawLine(window, point, b.velocityAtPoint(point) * 0.1 + point, sf::Color::Red);
				drawLine(window, point, (a.velocityAtPoint(point) - b.velocityAtPoint(point)) * 0.1 + point, sf::Color::Magenta);
				
				drawLine(window, point, (10 * normal) + point, sf::Color::Green);


				//cout << " : " << memo1 << " " << memo2 << "\n";
				icr = 1;

				if ((dot(a.velocityAtPoint(point) - b.velocityAtPoint(point), normal) < 0))
				//if ((dot(a.velocityAtPoint(point) - b.velocityAtPoint(point), normal) < 0) ^ !((i % 2 + j % 2) % 2))
				{
					//drawPoint(window, point, sf::Color::Blue);
					cout << "\t\t\tship skip\n";
					continue;
				}

				//drawPoint(window, point, sf::Color::Red);
				//drawLine(window, point, (100 * normal) + point, sf::Color::Red);
				cout << "ship hit!\n";
				double impulseUp = -(1 + elasticity) * dot(a.velocityAtPoint(point) - b.velocityAtPoint(point), normal);
				double Down1 = 0;
				double Down2 = 0;
				double Down3 = 0;
				Down1 += dot(normal, normal) * ((double)1 / a.mass + (double)1 / b.mass);
				//Down2 += sqr(dot(normal, (point - a.centerOfMass).perpendicular())) / a.momentOfInertia;
				//Down3 += sqr(dot(normal, (point - b.centerOfMass).perpendicular())) / b.momentOfInertia;
				double impulseDown = Down1 + Down2 + Down3;
				double impulse = impulseUp / impulseDown;

				a.linearVelocity += (impulse / a.mass) * normal;
				//a.angularVelocity += (dot((point - a.centerOfMass), impulse * normal)) / a.momentOfInertia;

				b.linearVelocity += (-impulse / b.mass) * normal;
				//b.angularVelocity += (dot((point - b.centerOfMass), -impulse * normal)) / b.momentOfInertia;

				//cout << "impulse\n";
				//cnt++;
				//if (cnt >= 2)
				//{
				//	killTime = true;
				//	return;
				//}
				//cout << "kill time!\n";
				//cout << " : " << fixed << setprecision(10) << it.first << " " << dt << "\n";
				//killTime = true;
				
				//cout << "do something!\n";
				// 
				//exit(0);
				//return;
			}
			/*
				Vector linearAcceleration = linearForce / mass;
				linearVelocity += linearAcceleration * dt;

				double angularAcceleration = torque / momentOfInertia;
				angularVelocity += angularAcceleration * dt;

				centerOfMass += linearVelocity * dt;
				angle = mod2PI(angle + angularVelocity * dt);
			*/
		}
	}
	assert(!killTime);
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

	plates[0].setPlate(Vector(50 + 30, 50 + 10), Vector(20 + 33, 50 + 16), Vector(60 + 40, 20 + 10), 1);
	plates[1].setPlate(Vector(50 - 30, 50 - 10), Vector(50 - 33, 50 - 16), Vector(50 - 40, 50 - 10), 1);
	plates[2].setPlate(Vector(70 - 30, 50 - 10), Vector(70 - 33, 50 - 16), Vector(70 - 40, 50 - 10), 1);

	{
		plates[0].linearVelocity = Vector(-1, 0) * 100;
		//plates[0].linearVelocity = Vector(-1, 0) * 0;
		plates[1].linearVelocity = Vector(0, +1) * 50;
		//plates[1].linearVelocity = Vector(0, +1) * 0;
		plates[2].linearVelocity = Vector(-0.9, +0.8) * 70;

		plates[0].angularVelocity = 10;
		plates[1].angularVelocity = 0.5;
		plates[2].angularVelocity = 1;
	}

	for (auto& plate : plates)
	{
		plate.mass = abs(cross(plate.relativeb - plate.relativea, plate.relativec - plate.relativea)) * 0.5 * 0.05;
	}

	vector<Vector> points;
	points.push_back(Vector(10, 10));
	points.push_back(Vector(30, 30));
	points.push_back(Vector(20, 20));
	points.push_back(Vector(40, 40));
	points.push_back(Vector(40, 40));
	points.push_back(Vector(40, 40));

	int hold = -1;

	while (window.isOpen())
	{
		icr = 0;
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		InputManager::update(window, {});

		if (InputManager::justPressed(sf::Mouse::Left))
		{
			sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
			Vector location(worldPos.x, worldPos.y);
	
			hold = -1;
			for (int i = 0; i < (int)points.size(); i++)
			{
				if (getDistance(points[i], location) < 1)
				{
					hold = i;
				}
			}
		}

		if (hold != -1)
		{
			assert(0 <= hold && hold < (int)points.size());
			sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
			Vector location(worldPos.x, worldPos.y);
			points[hold] = location;
		}

		if (InputManager::justReleased(sf::Mouse::Left))
		{
			hold = -1;
		}

		if (!InputManager::isPressed(sf::Mouse::Left))
		{
			assert(hold == -1);
		}

		float dt = frameClock.restart().asSeconds();
		dt = 0.0001;

		window.clear();
		updateSystem(plates, dt, window);
	


		{
			assert((int)points.size() == 6);

			//{
			//    Vector p = Segment(points[0], points[1]).nearestPoint(points[2]);
			//    //cout << " = " << p.x << " " << p.y << "\n";
			//    sf::CircleShape circleShape;
			//    circleShape.setRadius(1.5);
			//    circleShape.setOrigin(sf::Vector2f(1, 1) * circleShape.getRadius());
			//    circleShape.setPosition(sf::Vector2f(p.x, p.y));
			//    circleShape.setFillColor(sf::Color::Green);
			//    window.draw(circleShape);
			//}

			for (int j = 0; j <= 1; j++)
			{
				sf::VertexArray lv(sf::LineStrip, 4);
				for (int i = 0; i < 3; i++)
				{
					lv[i].position = sf::Vector2f(points[3 * j + i].x, points[3 * j + i].y);
				}
				lv[3] = lv[0];
				window.draw(lv);
			}

			Triangle t1(points[0], points[1], points[2]), t2(points[3], points[4], points[5]);
			if (1)
			{
				auto intersection = getIntersection(t1, t2);
				//cout << " = " << it.first << "\n";

				if (intersection)
				{
					sf::CircleShape circleShape;
					circleShape.setRadius(1.5);
					circleShape.setOrigin(sf::Vector2f(1, 1) * circleShape.getRadius());
					circleShape.setPosition(sf::Vector2f(intersection->x, intersection->y));
					circleShape.setFillColor(sf::Color::Red);
					window.draw(circleShape);
				}
			}
			if (1)
			{
				auto it = getNearest(t1, t2);
				{
					sf::CircleShape circleShape;
					circleShape.setRadius(1.5);
					circleShape.setOrigin(sf::Vector2f(1, 1) * circleShape.getRadius());
					circleShape.setPosition(sf::Vector2f(it.second.a.x, it.second.a.y));
					circleShape.setFillColor(sf::Color::Red);
					window.draw(circleShape);
				} 
				{
					sf::CircleShape circleShape;
					circleShape.setRadius(1.5);
					circleShape.setOrigin(sf::Vector2f(1, 1) * circleShape.getRadius());
					circleShape.setPosition(sf::Vector2f(it.second.b.x, it.second.b.y));
					circleShape.setFillColor(sf::Color::Red);
					window.draw(circleShape);
				}
				sf::VertexArray varray(sf::Lines, 2);
				varray[0].position = sf::Vector2f(it.second.a.x, it.second.a.y);
				varray[1].position = sf::Vector2f(it.second.b.x, it.second.b.y);
				window.draw(varray);
			}

			//cout << " = " << doIntersect(Segment(points[0], points[1]), Segment(points[2], points[3])) << "\n";
			
			for (int i = 0; i < (int)points.size(); i++)
			{
				sf::CircleShape circleShape;
				circleShape.setRadius(1);
				circleShape.setOrigin(sf::Vector2f(1, 1) * circleShape.getRadius());
				circleShape.setPosition(sf::Vector2f(points[i].x, points[i].y));
				window.draw(circleShape);
			}
		}

		for (auto& plate : plates)
		{
			drawTrianglePlate(plate, window);
		}
		
		Vector linearMomentum(0, 0);
		for (auto plate : plates)
		{
			linearMomentum += plate.linearVelocity * plate.mass;
			cout << plate.mass << " ";
		}
		cout << " | ";
		cout << "momentum = " << linearMomentum.norm() << "\n";

		if (killTime || 1)
		{
			Triangle t1(points[0], points[1], points[2]), t2(points[3], points[4], points[5]);
			auto it = getNearest(t1, t2);
			sf::VertexArray varray(sf::Lines, 2);
			varray[0].position = sf::Vector2f(Triangle::thePair.first.x, Triangle::thePair.first.y);
			Vector sum = Triangle::thePair.first + Triangle::thePair.second * 10;
			varray[1].position = sf::Vector2f(sum.x, sum.y);
			varray[0].color = varray[1].color = sf::Color::Green;
			window.draw(varray);
		}
		window.display();

		if (icr && 0)
		{
			while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				sf::Event event;
				while (window.pollEvent(event))
				{
					if (event.type == sf::Event::Closed)
					{
						window.close();
					}
				}
			}
			if (0)
			{
				bool was = 1, is = 0;
				while (true)
				{
					sf::Event event;
					while (window.pollEvent(event))
					{
						if (event.type == sf::Event::Closed)
						{
							window.close();
						}
					}
					is = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
					if (!was && is)
					{
						cout << "break!\n";
						break;
					}
					was = is;
				}
			}
			//sf::sleep(sf::seconds(1));
		}

	}

	return 0;
}