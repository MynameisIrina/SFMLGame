#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"

class Background
{
public:
    Background(const Player& pl);
    void Initialize(const sf::RenderWindow& window);
    void Update(float dt);
    void Draw(sf::RenderTarget& rt) const;
    void Move(float dt);
    void UpdateView();

private:
    // model
    const Player& player;
    float previousPlayerX;
    sf::Vector2f positionSprite1, positionSprite2, positionCurrentSprite;
    const float speed = 80.f;
    const float offsetPlacement = 1600.f;
    // view
    sf::Texture texture;
    sf::Sprite sprite1, sprite2, currentSprite;
    sf::Vector2u windowSize;
    sf::Vector2u textureSize;
};