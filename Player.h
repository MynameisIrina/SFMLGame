#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "Math.h"

class Player
{
private:
    // view
    sf::Texture texture;
    sf::Sprite sprite;
    int currentAnim = 0;
    sf::RectangleShape boundingRec;
    // model
    float speed = 0.05f;
    sf::Vector2f velocity = sf::Vector2f(100.f, 10.f);;
    float animationTimer = 0.f;
    float animationInterval = 0.1f;
    sf::Vector2f position;
    float gravity = 600.f;
    float collisionGravityFactor = 0.1f;
    bool onGround = true;
    bool jumped = false;
    bool stopped = true;
    Math math;

public:
    void Initialize(const sf::Vector2f &pos);
    void Move(bool moveRight, bool moveLeft, float dt, float leftBound);
    void Draw(std::shared_ptr<sf::RenderTarget> rt) const;
    sf::Vector2f GetPosition() const;
    void UpdateView(bool moveRight, bool moveLeft);
    void Update(float dt, std::vector<sf::RectangleShape> tiles);
    void Jump(bool jumped, float dt);
    void ResetAnimation(int animYIndex);
    void CheckCollision(std::vector<sf::RectangleShape> tiles);
    void CalculateCurrAnimation(float dt);
};