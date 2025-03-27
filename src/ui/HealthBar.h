#pragma once
#include "graphics/TextureLoader.h"
#include "camera/Camera.h"

class Player;

class HealthBar
{

public:
    struct Heart
    {
        sf::Sprite sprite;
        sf::Vector2f position;
        bool isActive;
        bool shouldPulse;
    };

    HealthBar(const std::shared_ptr<TextureLoader>& txLoader);
    void Initialize(const std::shared_ptr<Player>& player, const std::shared_ptr<Camera>& camera);
    void Draw(const std::shared_ptr<sf::RenderTarget>& rt) const;
    void Update(const std::shared_ptr<Player>& player, const std::shared_ptr<Camera>& camera);
    void UpdateView();
    void HandlePulseAnimation(const std::shared_ptr<Player> &player);

private:
    std::vector<Heart> hearts;
    std::shared_ptr<TextureLoader> txLoader;
    const float offsetX = 210.f;
    const float offsetY = 25.f;
    const float scale = 2;
    int currentHealth = 0;
    int heartSpriteSize = 32;
    const float pulseInterval = 0.5f;
    sf::Clock animationTimer;
    float pulseDuration = 3.f;
    bool pulsing = false;
    bool animationActive = false;
    float lastPulseTime = 0.f;
    int heartsAdded = 0;

};