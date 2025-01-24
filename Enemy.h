#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"

class Enemy {
public:
    Enemy();
    virtual ~Enemy() = default;

    virtual void Initialize(sf::Sprite& sprite, sf::Vector2f startPosition, int health, int damage) = 0;
    virtual void Update(const std::shared_ptr<Player> player, float dt);
    virtual void UpdateView() = 0;
    virtual void UpdateAnimation(float dt) = 0;

    void TakeDamage(int amount);
    virtual void Draw(const std::shared_ptr<sf::RenderWindow> window) const;

protected:
    sf::Sprite sprite;
    sf::Vector2f position;
    int health;
    int damage;
    bool isDead;
};
