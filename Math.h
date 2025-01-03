#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Math
{
public:
    static bool CheckRectCollision(sf::FloatRect rect1, sf::FloatRect rect2);
    static bool CheckGroundCollision(sf::FloatRect rect1, sf::FloatRect rect2);
};