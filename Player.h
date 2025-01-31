#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "Math.h"
#include "TextureLoader.h"
#include "HealthBar.h"
#include "Tile.h"
#include "RayCast.h"
#include "ProjectilePool.h"

class Player
{
public:

    enum PlayerCondition
    {
        Normal,
        Protected,
        Dead
    };

    enum State
    {
        Moving = 1 << 0,
        Jumping = 1 << 1,
        Shooting = 1 << 2,
        Respawning = 1 << 3,
        Blinking = 1 << 4
    };

    

private:

    // Private data
    std::shared_ptr<TextureLoader> txLoader;
    sf::Sprite sprite;
    std::shared_ptr<HealthBar> healthBar;
    int currentAnim = 0;
    sf::RectangleShape boundingBoxPlayer;
    RayCast::Ray ray{sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 0.f)};
    sf::Vector2f velocity = sf::Vector2f(100.f, 10.f);
    sf::Vector2f saveLastPos;
    sf::Vector2f respawnPos;
    sf::Vector2f maxPosition;
    sf::Vector2f position;
    float scale;
    PlayerCondition condition;
    sf::Clock loseLifeCooldown;
    sf::Clock respawnTimer;
    sf::Clock blinkingTimer;
    PlayerCondition currentState;
    ProjectilePool &projectilePool;
    int state;
    float positionThresholdY;
    int projectilesCount = 0;
    int maxProjectileCount = 0;

    // const variables
    const float epsilon = 3.0f;
    const float horizontalVelocity = 100.0f;
    const float speed = 0.05f;
    const float gravity = 600.f;
    const float rebirth_animation_duration = 0.8f;
    const float rebirth_animation_interval = 0.1f;
    const int maxFrames = 8;
    const float animationInterval = 0.1f;
    const float loseLifeDelay = 2.0f;
    const float blinkingInterval = 0.1f;
    const int tileSize = 32;
    const int boundingBoxOffsetX = 15;
    const float projectileOffsetX = 10.f;
    const float projectileVelocity = 500.f;
    const float jumpVelocity = 280.f;

    // Flags
    bool collisionGround = false;
    bool collisionSide = false;
    bool collisionTop = false;
    bool collisionObstacle = false;
    bool isRespawnTimerRestarted = false;
    bool isVisible = true;
    bool isShooting = false;

    // Animation
    float animationTimer = 0.f;
    float rebornAnimationTimer = 0.f;

    // Health
    int health = 0;
    int maxHealth = 0;

public:
    // Constructor
    Player(const std::shared_ptr<TextureLoader> &txLoader, ProjectilePool &projectilePool);

    // Initialization
    void Initialize(const sf::Vector2f position, const int maxHealth, const int projectilesAmount, const float scale, const float positionThresholdY);

    // Update
    void Update(const bool moveRight,const bool moveLeft, const bool shoot, const float leftBound, const bool respawn, const float dt, const std::vector<Tile> &tiles);
    void UpdateView(const bool moveRight, const bool moveLeft);

    // Movement
    void HandleFalling();
    void HandleHorizontalMovement(const float dt, const float leftBound);
    void HandleVerticalMovement(const float dt);
    void Jump(const bool jumped, const float dt);
    void HandleShooting(const bool shoot, const float dt);
    int CalculateDirection();

    // Collisions
    void CheckCollisionGround(const std::vector<Tile> &tiles);
    void CheckCollisionSide(const std::vector<Tile> &tiles);
    void HandleObstacleCollision();
    void HandleEnemyCollision();
    sf::RectangleShape CreateBoundingBox();

    // Animation
    void CalculateCurrentAnimation(const float dt);
    void ResetAnimation(const int animYIndex);
    void HandleBlinking();

    // Respawn
    void HandleRespawn();

    // Health
    void IncreaseHealth(); 
    void DecreaseHealth(); 
    int GetHealth() const; 
    int GetMaxHealth() const;
    bool IsPlayerProtected();
    void LoseLife();
    void GainLife();

    // Other
    void Draw(const std::shared_ptr<sf::RenderTarget> rt) const;
    sf::Vector2f GetPosition() const;
    sf::Vector2f GetMaxPosition() const;
    int GetProjectilesCount();
    std::vector<Projectile*> GetActiveProjectiles() const;
    sf::RectangleShape GetBoundingBox() const;
    void ResetProjectilesCount();

    // Utilities
    static void DrawRay(const std::shared_ptr<sf::RenderTarget> &rt, const sf::Vector2f start, const sf::Vector2f end, sf::Color color = sf::Color::Red);

    // States
    void SetState(State stateToSet);
    void ClearState(State stateToClear);
    bool IfStateActive(State stateToCheck);
    void InputToState(bool moveRight, bool moveLeft, bool shoot, bool respawn);
};
