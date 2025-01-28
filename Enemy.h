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

    virtual void Initialize(sf::Sprite& sprite, sf::Vector2f startPosition, int health, int damage) = 0;
    virtual void Update(const std::shared_ptr<Player>& player, const std::shared_ptr<Camera>& camera, float dt);
    virtual void UpdateView() = 0;
    virtual void UpdateAnimation(float dt) = 0;
    virtual void HandleDeath() = 0;


    void TakeDamage(int amount);
    State GetState();
    sf::RectangleShape GetBoundingBox();
    virtual void Draw(const std::shared_ptr<sf::RenderWindow> window) const;

protected:
    State state;
    sf::Sprite sprite;
    sf::Vector2f position;
    sf::RectangleShape boundingBox;
    int health;
    int damage;
};
