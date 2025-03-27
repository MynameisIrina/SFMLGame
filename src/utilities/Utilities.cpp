#include "utilities/Utilities.h"
#include <iostream>


sf::RectangleShape Utilities::CreateBoundingBox(const sf::Sprite& sprite, const sf::Vector2f& position)
{
    const sf::FloatRect spriteBounds = sprite.getGlobalBounds();
    const float halfWidth = spriteBounds.width * 0.5f;
    const float halfHeight = spriteBounds.height * 0.5f;

    sf::RectangleShape boundingBox(sf::Vector2f(spriteBounds.width, spriteBounds.height));;
    boundingBox.setFillColor(sf::Color::Transparent);
    boundingBox.setOutlineColor(sf::Color::Blue);
    boundingBox.setOutlineThickness(1);
    boundingBox.setPosition(position);
    boundingBox.setOrigin(halfWidth, halfHeight);
    return boundingBox;
}

sf::CircleShape Utilities::CreateBoundingCircle(const sf::Sprite &sprite, const sf::Vector2f& position)
{
    const sf::FloatRect spriteBounds = sprite.getGlobalBounds();
    const float radius = std::max(spriteBounds.width, spriteBounds.height) * 0.5f;
    const float halfWidth = spriteBounds.width * 0.5f;
    const float halfHeight = spriteBounds.height * 0.5f;

    sf::CircleShape boundingCircle(radius);;
    boundingCircle.setFillColor(sf::Color::Transparent);
    boundingCircle.setOutlineColor(sf::Color::Blue);
    boundingCircle.setOutlineThickness(1);
    boundingCircle.setPosition(position);
    boundingCircle.setOrigin(halfWidth, halfHeight);
    return boundingCircle;
}

sf::Color Utilities::GetOpaqueColor(const sf::Color& color) 
{
    return sf::Color(color.r, color.g, color.b, color.a);
}

sf::Color Utilities::GetTransparentColor(const sf::Color& color)
{
    return sf::Color(color.r, color.g, color.b, 0);
}



