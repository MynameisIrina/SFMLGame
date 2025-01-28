#include "Utilities.h"
#include <iostream>


sf::RectangleShape Utilities::CreateBoundingBox(sf::Sprite& sprite, sf::Vector2f position)
{
    sf::FloatRect spriteBounds = sprite.getGlobalBounds();
    sf::RectangleShape boundingBox(sf::Vector2f(spriteBounds.width, spriteBounds.height));;
    boundingBox.setFillColor(sf::Color::Transparent);
    boundingBox.setOutlineColor(sf::Color::Blue);
    boundingBox.setOutlineThickness(1);
    boundingBox.setPosition(position);
    boundingBox.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
    return boundingBox;
}

sf::CircleShape Utilities::CreateBoundingCircle(sf::Sprite &sprite, sf::Vector2f position)
{
    sf::FloatRect spriteBounds = sprite.getGlobalBounds();
    float radius = std::max(spriteBounds.width, spriteBounds.height) / 2.0f;
    sf::CircleShape boundingCircle(radius);;
    boundingCircle.setFillColor(sf::Color::Transparent);
    boundingCircle.setOutlineColor(sf::Color::Blue);
    boundingCircle.setOutlineThickness(1);
    boundingCircle.setPosition(position);
    boundingCircle.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
    return boundingCircle;
}

sf::Color Utilities::GetOpaqueColor()
{
    return  sf::Color(255, 255, 255, 255);
}

sf::Color Utilities::GetTransparentColor()
{
    return  sf::Color(255, 255, 255, 0);
}
