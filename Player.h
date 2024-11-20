#pragma once
#include <SFML/Graphics.hpp>

class Player
{
private:
    sf::Texture texture;
    sf::Sprite sprite;
    float speed;
    sf::Vector2f velocity;
    int currentAnim;
    float animationTimer;
    float animationInterval;

public:
    void Initialize();
    void Load(const sf::Vector2f& pos);
    void Move(float& dt);
    void Draw(sf::RenderTarget& rt) const;
    void Update();
    bool isMovingRight;
};