#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"
#include "CollectibleManager.h"


class Enemy {
public:

    enum State
    {
        Alive,
        Dead
    };

    Enemy(const std::shared_ptr<AudioManager>& audioManager);
    virtual ~Enemy() = default;

    virtual void Initialize(const sf::Sprite &sprite, const sf::Vector2f startPosition, const int health, const int damage) = 0;
    virtual void Update(const std::shared_ptr<Player>& player, const std::shared_ptr<Camera>& camera, float dt);
    virtual void UpdateView() = 0;
    virtual void UpdateAnimation(const float dt) = 0;
    virtual void HandleDeath(const std::shared_ptr<CollectibleManager>& collectibleManager) = 0;
    virtual void Respawn();

    void TakeDamage(const int amount);
    State GetState() const;
    sf::RectangleShape GetBoundingBox() const; 
    virtual void Draw(const std::shared_ptr<sf::RenderWindow>& window) const;

protected:
    std::shared_ptr<AudioManager> audioManager;
    State state;
    sf::Sprite sprite;
    sf::Vector2f startPosition;
    sf::Vector2f position;
    sf::RectangleShape boundingBox;
    sf::RectangleShape initialBoundingBox;
    int health;
    int maxHealth;
    int damage;
    bool handledDeath = false;
};