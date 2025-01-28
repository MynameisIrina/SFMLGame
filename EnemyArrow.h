#pragma once
#include "Enemy.h"
#include "ArrowPool.h"
#include <iostream>

class EnemyArrow: public Enemy
{

public:
    EnemyArrow(ArrowPool arrowPool);
    void Initialize(sf::Sprite &sprite, sf::Vector2f startPosition, int health, int damage) override;
    void Update(const std::shared_ptr<Player>& player,const std::shared_ptr<Camera>& camera, float dt) override;
    void UpdateAnimation(float dt) override;
    void UpdateView() override;
    void ShootArrow();
    void Draw(const std::shared_ptr<sf::RenderWindow> window) const override;
    void HandleShooting(const std::shared_ptr<Camera> camera);
    void HandleRotation(const std::shared_ptr<Player>& player);
    void HandleCollision(const std::shared_ptr<Player>& player);
    void HandleDeath() override;
    State GetState();

private:
    ArrowPool arrowPool;
    float animationTimer = 0.f;
    const float animationInterval = 0.3f;
    int currentAnim = 0;
    const int maxFrames = 7;
    const int shootingFrame = 5;
    const int frameSize = 128;
    const float shootingOffset = 20.f;
    const float velocity = 170.f;
    float rotation = 0.f;
    const float scale = 0.45f;
    bool isShooting = false;
};