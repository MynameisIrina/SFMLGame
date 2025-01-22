#pragma once
#include "Enemy.h"
#include <iostream>

class EnemyArrow: public Enemy
{
public:
    void Initialize(sf::Sprite &sprite, sf::Vector2f startPosition, int health, int damage) override;
    void Update(const std::shared_ptr<Player> player, float dt) override;
    void UpdateAnimation(float dt) override;
    void UpdateView() override;

private:
    float animationTimer = 0.f;
    float animationInterval = 0.3f;
    int currentAnim = 0;
    int max_frames = 7;
    float rotation;
};