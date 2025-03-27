#pragma once
#include "enemies/Enemy.h"

class Eagle: public Enemy
{
public:
    Eagle(const std::shared_ptr<AudioManager>& audioManager);
    void Initialize(const sf::Sprite &sprite, const sf::Vector2f& position, const int health, const int damage) override;
    void Update(const std::shared_ptr<Player>& player,const std::shared_ptr<Camera>& camera, const float dt) override;
    void UpdateAnimation(const float dt) override;
    void UpdateView() override;
    void Draw(const std::shared_ptr<sf::RenderWindow>& window) const override;
    void HandleDeath(const std::shared_ptr<CollectibleManager>& collectibleManager) override;
    void SetMovementDirection(const int direction);
    void HandleHorizontalMovement(const std::shared_ptr<Camera>& camera, const float dt);
    void HandleCollision(const std::shared_ptr<Player> &player);
    int GetDirection() const;
    sf::Vector2f GetPosition() const;

    enum class State
    {
        Idle,
        Attacking,
        FlyingAway
    };

private:

    int currentState;
    float animationTimer = 0.f;
    const float animationInterval = 0.15f;
    int currentAnim = 0;
    const int maxFrames = 3;
    const int frameSize = 128;
    int movementDirection = 1;
    const int speed = 120.f;
    const float scale = 1.3f;
    const float boundingBoxOffsetY = 8.f;
    const float scalar = 0.8f;
};