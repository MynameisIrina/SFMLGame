#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"

class Enemy {
public:

    enum State
    {
        Alive,
        Dead
    };

    Enemy();
    virtual ~Enemy() = default;

    virtual void Initialize(const sf::Sprite &sprite, const sf::Vector2f startPosition, const int health, const int damage) = 0;
    virtual void Update(const std::shared_ptr<Player>& player, const std::shared_ptr<Camera>& camera, float dt);
    virtual void UpdateView() = 0;
    virtual void UpdateAnimation(float dt) = 0;
    virtual void HandleDeath() = 0;


    void TakeDamage(int amount);
    State GetState() const;
    sf::RectangleShape GetBoundingBox() const; 
    virtual void Draw(const std::shared_ptr<sf::RenderWindow> window) const;

protected:
    State state;
    sf::Sprite sprite;
    sf::Vector2f position;
    sf::RectangleShape boundingBox;
    int health;
    int damage;
};
