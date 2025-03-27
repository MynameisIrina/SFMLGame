#pragma once
#include "graphics/TextureLoader.h"
#include "camera/Camera.h"
#include "player/Player.h"

class ProjectileBar
{

public:
    ProjectileBar() = default;
    void Initialize(const std::shared_ptr<Player>& player, const std::shared_ptr<Camera>& camera);
    void Draw(const std::shared_ptr<sf::RenderTarget>& rt) const;
    void Update(const std::shared_ptr<Player>& player, const std::shared_ptr<Camera>& camera, const float dt);
    void UpdateView();
    void ReduceAmount(const float amountToReduce);
    void IncrementAmount(const float amountToIncrease, const float dt);
    void UpdateAnimation();
    void AddVisualEffects(const float dt);
    void ResetBar();
private:
    std::shared_ptr<TextureLoader> txLoader;
    sf::RectangleShape projectileBar;
    sf::RectangleShape backgroundBar;
    std::vector<sf::RectangleShape> verticalLinesOnBar;
    const float segmentLineWidth = 2.f;
    int segmentsAmount = 0;
    float currentAmount = 0.f;
    const float offsetX = 380.f;
    const float offsetY = 10.f;
    const float scale = 2.f;
    const float barHeight = 10.f;
    sf::Vector2f position = sf::Vector2f(0.f,0.f);
    int previousProjectileCount = 0;
    float amountToReduce = 0.f;
    const float amountToIncrease = 12.f;
    const float maxAmount = 60.f;
    sf::Clock blinkingTimer;
    float blinkingInterval = 0.4f;
    bool isVisible = false;
    const sf::Color color = sf::Color(150, 0, 0, 255);
    const sf::Color blinkingColor = sf::Color(50,50,50, 255);
    int maxProjectiles;
    sf::Clock reloadTimer;
    const float reloadTime = 2.f;
    float currentVisualAmount = 0.f;
    bool isResetting = true;
};