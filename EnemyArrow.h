#pragma once
#include "Enemy.h"
#include "ArrowPool.h"
#include <iostream>

class EnemyArrow: public Enemy
{
    
public:
    EnemyArrow(ArrowPool&& arrowPool);
    void Initialize(sf::Sprite &sprite, sf::Vector2f startPosition, int health, int damage) override;
    void Update(const std::shared_ptr<Player> player, float dt) override;
    void UpdateAnimation(float dt) override;
    void UpdateView() override;
    void ShootArrow();
    void Draw(const std::shared_ptr<sf::RenderWindow> window) const override;

private:
    ArrowPool arrowPool;
    float animationTimer = 0.f;
    const float animationInterval = 0.3f;
    int currentAnim = 0;
    const int max_frames = 7;
    const int shooting_frame = 5;
    const float shootingOffset = 20.f;
    const float velocity = 170.f;
    float rotation = 0.f;
    const float scale = 0.45f;
};