#pragma once
#include <SFML/Graphics.hpp>

class Utilities
{

public:
    static sf::RectangleShape CreateBoundingBox(const sf::Sprite& sprite, const sf::Vector2f& position);
    static sf::CircleShape CreateBoundingCircle(const sf::Sprite& sprite, const sf::Vector2f& position);
    static sf::Color GetOpaqueColor(const sf::Color& color);
    static sf::Color GetTransparentColor(const sf::Color& color);
};