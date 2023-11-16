#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cassert>

class InputManager
{
public:
    static bool isPressed(sf::Keyboard::Key key);
    static bool justPressed(sf::Keyboard::Key key);
    static bool justReleased(sf::Keyboard::Key key);

    static bool isPressed(sf::Mouse::Button button);
    static bool justPressed(sf::Mouse::Button button);
    static bool justReleased(sf::Mouse::Button button);

    static sf::Vector2f getMouseWorldPosition();

private:

    static void update(const sf::RenderWindow& window, const std::vector<sf::Event>& currentEvents);

    static std::vector<bool> sm_wasKeyPressed;
    static std::vector<bool> sm_isKeyPressed;

    static std::vector<bool> sm_wasButtonPressed;
    static std::vector<bool> sm_isButtonPressed;

    static sf::Vector2f sm_mousePosition;

    friend int main();

private:
    InputManager();
};

