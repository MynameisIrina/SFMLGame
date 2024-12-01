#pragma once
#include <SFML/Graphics.hpp>

class Player
{
private:
    // view
    sf::Texture texture;
    sf::Sprite sprite;
    int currentAnim;
    // model
    float speed;
    sf::Vector2f velocity;
    float animationTimer;
    float animationInterval;
    sf::Vector2f position;

public:
    void Initialize(const sf::Vector2f &pos);
    void Move(bool moveRight, bool moveLeft);
    void Draw(sf::RenderTarget& rt) const;
    sf::Vector2f GetPosition() const;
    void UpdateView(bool moveRight, bool moveLeft);
    void Update(float dt);
};