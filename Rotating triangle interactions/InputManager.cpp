#include "InputManager.h"

std::vector<bool> InputManager::sm_wasKeyPressed(sf::Keyboard::KeyCount, false);
std::vector<bool> InputManager::sm_isKeyPressed(sf::Keyboard::KeyCount, false);

std::vector<bool> InputManager::sm_wasButtonPressed(sf::Mouse::ButtonCount, false);
std::vector<bool> InputManager::sm_isButtonPressed(sf::Mouse::ButtonCount, false);

sf::Vector2f InputManager::sm_mousePosition;

sf::Vector2f InputManager::getMouseWorldPosition()
{
    return sm_mousePosition;
}

bool InputManager::isPressed(sf::Keyboard::Key key)
{
    return sm_isKeyPressed[static_cast<int>(key)];
}

bool InputManager::justPressed(sf::Keyboard::Key key)
{
    return sm_isKeyPressed[static_cast<int>(key)] && !sm_wasKeyPressed[static_cast<int>(key)];
}

bool InputManager::justReleased(sf::Keyboard::Key key)
{
    return !sm_isKeyPressed[static_cast<int>(key)] && sm_wasKeyPressed[static_cast<int>(key)];
}

bool InputManager::isPressed(sf::Mouse::Button button)
{
    return sm_isButtonPressed[static_cast<int>(button)];
}

bool InputManager::justPressed(sf::Mouse::Button button)
{
    return sm_isButtonPressed[static_cast<int>(button)] && !sm_wasButtonPressed[static_cast<int>(button)];
}

bool InputManager::justReleased(sf::Mouse::Button button)
{
    return !sm_isButtonPressed[static_cast<int>(button)] && sm_wasButtonPressed[static_cast<int>(button)];
}

void InputManager::update(const sf::RenderWindow& window, const std::vector<sf::Event>& currentEvents)
{
    sm_wasKeyPressed = sm_isKeyPressed;
    sm_wasButtonPressed = sm_isButtonPressed;

    assert((int)sm_isKeyPressed.size() == sf::Keyboard::KeyCount);
    assert((int)sm_isButtonPressed.size() == sf::Mouse::ButtonCount);

    for (int i = 0; i < sf::Keyboard::KeyCount; i++)
    {
        sm_isKeyPressed[i] = sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(i));
    }

    for (int i = 0; i < sf::Mouse::ButtonCount; i++)
    {
        sm_isButtonPressed[i] = sf::Mouse::isButtonPressed(static_cast<sf::Mouse::Button>(i));
    }

    sm_mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getView());
}

InputManager::InputManager()
{

}
