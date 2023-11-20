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

int main()
{
    sf::RenderWindow window(sf::VideoMode(900, 900), "Rotating triangle interactions");

    sf::View view;
    view.setSize(sf::Vector2f(100, -100));
    view.setCenter(sf::Vector2f(50, 50));
    window.setView(view);

    sf::Clock frameClock;

    TrianglePlate plate;
    plate.setPlate(Vector(50 + 30, 50 + 10), Vector(50 + 33, 50 + 16), Vector(50 + 40, 50 + 10), 1);

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
        //dt = 0.00001;


        Vector a = plate.centerOfMass + plate.relativea.rotate(plate.angle);

        //plate.applyForce(a, Vector(0, -0.001));
        Vector dif = Vector(0, 1);
        plate.applyForce(plate.centerOfMass + Vector(0, 1) * 0.1, Vector(-1, 0));
        plate.applyForce(plate.centerOfMass - Vector(0, 1) * 0.1, Vector(+1, 0));
        // forces couple

        plate.update(dt);


        window.clear();

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
            if (0)
            {
                auto it = getIntersection(t1, t2);
                //cout << " = " << it.first << "\n";

                if (it.first)
                {
                    sf::CircleShape circleShape;
                    circleShape.setRadius(1.5);
                    circleShape.setOrigin(sf::Vector2f(1, 1) * circleShape.getRadius());
                    circleShape.setPosition(sf::Vector2f(it.second.x, it.second.y));
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
                    circleShape.setPosition(sf::Vector2f(it.second.first.x, it.second.first.y));
                    circleShape.setFillColor(sf::Color::Red);
                    window.draw(circleShape);
                } 
                {
                    sf::CircleShape circleShape;
                    circleShape.setRadius(1.5);
                    circleShape.setOrigin(sf::Vector2f(1, 1) * circleShape.getRadius());
                    circleShape.setPosition(sf::Vector2f(it.second.second.x, it.second.second.y));
                    circleShape.setFillColor(sf::Color::Red);
                    window.draw(circleShape);
                }
                sf::VertexArray varray(sf::Lines, 2);
                varray[0].position = sf::Vector2f(it.second.first.x, it.second.first.y);
                varray[1].position = sf::Vector2f(it.second.second.x, it.second.second.y);
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

        drawTrianglePlate(plate, window);
        window.display();

    }

    return 0;
}