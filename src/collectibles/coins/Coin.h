#pragma once
#include "collectibles/Collectible.h"


class Coin : public Collectible
{

public:
    void ConvertEnemyToCoin(sf::Sprite &sprite, const sf::Vector2f& position);
    void Initialize(sf::Sprite &sprite, const sf::Vector2f& position);
    void Update(const std::shared_ptr<Player>& player, const float dt) override;
    void UpdateAnimation(float dt);
    void UpdateView();

private:
    float animationTimer = 0.f;
    const float animationInterval = 0.15f;
    int currentAnim = 0;
    const int maxFrames = 13;
    const int frameSize = 32;
};