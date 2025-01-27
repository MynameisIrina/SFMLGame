#pragma once
#include <SFML/Graphics.hpp>
#include "TextureLoader.h"
#include <iostream>

class Obstacle
{
public:

    void Initialize(sf::Sprite& sprite, sf::Vector2f startPosition, float speed, float minX, float maxX);
    void Draw(const std::shared_ptr<sf::RenderTarget>& rt) const;
    void MoveObstacles(float dt);
    void MoveObstacle(float dt);
    void UpdateTexture();
    sf::Vector2f GetPosition() const;
    sf::Sprite GetSprite() const;
    sf::RectangleShape GetBoundingBox() const;
    void CreateVisualLine(float minX, float maxX, float minY, float maxY);
    void Draw(const std::shared_ptr<sf::RenderWindow> window) const;

private:
    std::shared_ptr<TextureLoader> txLoader;
    sf::RectangleShape boundingBox;
    sf::Sprite sprite;
    sf::Vector2f position;
    float rotation;
    float speed;
    float rotationSpeed = 0.1f;
    bool movingForward = true;;
    float maxX;
    float minX;
    sf::Vertex line[2];
};