#pragma once
#include <SFML/Graphics.hpp>

class Utilities
{

public:
    static sf::RectangleShape CreateBoundingBox(sf::Sprite& sprite, sf::Vector2f position);
    static sf::CircleShape CreateBoundingCircle(sf::Sprite& sprite, sf::Vector2f position);
    static sf::Color GetOpaqueColor();
    static sf::Color GetTransparentColor();
};