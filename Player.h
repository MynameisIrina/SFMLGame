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

private:
    // Private data
    std::shared_ptr<TextureLoader> txLoader;
    sf::Sprite sprite;
    std::shared_ptr<HealthBar> healthBar;
    int currentAnim = 0;
    sf::RectangleShape boundingBoxPlayer;
    RayCast::Ray ray{sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 0.f)};
    sf::Vector2f velocity = sf::Vector2f(100.f, 10.f);
    sf::Vector2f saveLastPos = sf::Vector2f(35.f, 20.f);
    sf::Vector2f respawnPos = sf::Vector2f(35.f, 20.f);
    sf::Vector2f maxPosition = sf::Vector2f(35.f, 20.f);
    sf::Vector2f position;
    float scale;
    PlayerCondition condition;
    sf::Clock loseLifeCooldown;
    sf::Clock respawnTimer;
    sf::Clock blinkingTimer;
    PlayerCondition currentState;
    ProjectilePool &projectilePool;

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
    const int rectWidthPlayer = 22;
    const int rectHeightPlayer = 16;
    const int playerOffset_x = 4;
    const int tileSize = 32;
    const int boundingBoxOffsetX = 15;
    const float projectileOffsetX = 10.f;
    const float projectileVelocity = 500.f;
    const float jumpVelocity = 280.f;

    // Flags
    bool isJumping = false;
    bool collisionGround = false;
    bool collisionSide = false;
    bool collisionTop = false;
    bool respawn = false;
    bool moveRight = false;
    bool moveLeft = false;
    bool atRespawnPos = false;
    bool collisionObstacle = false;
    bool isRespawnTimerRestarted = false;
    bool isVisible = true;
    bool isBlinking = false;
    bool shoot = false;
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
    void Initialize(sf::Vector2f pos, int maxHealthRef, float scale);

    // Update
    void Update(bool moveRight, bool moveLeft, bool shoot, float leftBound, bool respawn, float dt, std::vector<Tile> &tiles);
    void UpdateView(bool moveRight, bool moveLeft);

    // Movement
    void HandleFalling();
    void HandleHorizontalMovement(float dt, float leftBound);
    void HandleVerticalMovement(float dt);
    void Jump(bool jumped, float dt);
    void HandleShooting(bool shoot, float dt);
    int CalculateDirection();

    // Collisions
    void CheckCollisionGround(const std::vector<Tile> &tiles);
    void CheckCollisionSide(const std::vector<Tile> &tiles);
    void HandleObstacleCollision();
    void HandleEnemyCollision();
    sf::RectangleShape CreateBoundingBox();

    // Animation
    void CalculateCurrentAnimation(float dt);
    void ResetAnimation(int animYIndex);
    void HandleBlinking();

    // Respawn
    void Respawn();
    void HandleRespawn(bool respawn);

    // Health
    void IncreaseHealth(); 
    void DecreaseHealth(); 
    int GetHealth() const; 
    int GetMaxHealth() const;
    bool IsPlayerProtected();
    void LoseLife();
    void GainLife();

    // Other
    void Draw(const std::shared_ptr<sf::RenderTarget> rt);
    sf::Vector2f GetPosition() const;
    sf::Vector2f GetMaxPosition() const;
    std::vector<Projectile*> GetActiveProjectiles();
    sf::RectangleShape GetBoundingBox();
    bool IsRespawn() const;
    bool IsMoveLeft() const;

    // Utilities
    static void DrawRay(const std::shared_ptr<sf::RenderTarget> &rt, const sf::Vector2f start, const sf::Vector2f end, sf::Color color = sf::Color::Red);

};
