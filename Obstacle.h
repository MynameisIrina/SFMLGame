#pragma once
#include <SFML/Graphics.hpp>
#include "TextureLoader.h"
#include <iostream>

class Obstacle
{
public:
    Obstacle(std::shared_ptr<TextureLoader> txLoaderRef);
    void Initialize(sf::Vector2f startPosition, float speed, float minX, float maxX);
    void Draw(std::shared_ptr<sf::RenderTarget> rt) const;
    void MoveObstacle(float dt);
    void UpdateTexture();
    sf::Vector2f GetPosition() const;
    sf::Sprite GetSprite() const;

private:
    std::shared_ptr<TextureLoader> txLoader;
    sf::Sprite sprite;
    sf::Vector2f position;
    float speed = 50.f;
    bool movingForward = true;;
    float maxX;
    float minX;
};