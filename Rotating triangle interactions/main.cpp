#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

int main()
{
  sf::RenderWindow window(sf::VideoMode(500, 500), "SFML works!");
  sf::CircleShape shape(100.f);
  shape.setFillColor(sf::Color::Green);

  sf::Clock frameClock;

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
      // left key is pressed: move our character
      shape.move(1, 1);
      sf::sleep(sf::seconds(0.1));
    }

    float dt = frameClock.restart().asSeconds();

    // v = dp/dt  --->   dp = v * dt
    // position += dp
    ///shape.move(100.0f * sf::Vector2f(cos(3.14 / 6), sin(3.14 / 6)) * dt);

    cout << 1 / dt << endl;

    window.clear();
    window.draw(shape);
    window.display();

    // sf::sleep(sf::seconds(0.01));
  }

  return 0;
}