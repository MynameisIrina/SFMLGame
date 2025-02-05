#pragma once
#include "Collectible.h"
#include "Enemy.h"

class Coin : public Collectible
{

public:
    void Pickup(const std::shared_ptr<Player> &player) override;
    void ConvertEnemyToCoin(sf::Sprite &sprite, const sf::Vector2f position);
    void Initialize(sf::Sprite &sprite, const sf::Vector2f position);
    void Update(float dt) override;
    void UpdateAnimation(float dt);
    void UpdateView();

private:
    float animationTimer = 0.f;
    const float animationInterval = 0.15f;
    int currentAnim = 0;
    const int maxFrames = 13;
    const int frameSize = 32;
};