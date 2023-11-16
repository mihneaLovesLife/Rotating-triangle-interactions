/*
What is the purpose of this project?
I want to be able to handle physically accurate interactions of rotating triangles. (1)

*/
#include <SFML/Graphics.hpp>
#include <iostream>
#include "TrianglePlate.h"
#include "Vector.h"

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
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    sf::View view;
    view.setSize(sf::Vector2f(100, -100));
    view.setCenter(sf::Vector2f(50, 50));
    window.setView(view);

    sf::Clock frameClock;

    TrianglePlate plate;
    plate.setPlate(Vector(50 + 30, 50 + 10), Vector(50 + 33, 50 + 16), Vector(50 + 40, 50 + 10), 1);
    
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

        float dt = frameClock.restart().asSeconds();
        dt = 0.00001;

        Vector a = plate.centerOfMass + plate.relativea.rotate(plate.angle);
        
        //plate.applyForce(a, Vector(0, -0.001));
        Vector dif = Vector(0, 1);
        plate.applyForce(plate.centerOfMass + Vector(0, 1) * 0.1, Vector(-1, 0));
        plate.applyForce(plate.centerOfMass - Vector(0, 1) * 0.1, Vector(+1, 0));
        // forces couple
        plate.update(dt);

        window.clear();
        drawTrianglePlate(plate, window);
        window.display();

    }

    return 0;
}