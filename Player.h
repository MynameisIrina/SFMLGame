#pragma once
#include <SFML/Graphics.hpp> // Needed for sf::Sprite, Vector2f, etc. in the interface
#include <memory>  // Needed for shared_ptr/unique_ptr in the interface
#include "RayCast.h"

class Enemy;
class RespawnManager;
class TextureLoader;
class AudioManager;
class ProjectilePool;
class HealthBar;
class Projectile;
class Camera;

class Player
{
public:

    struct CollisionInfo
    {
        float overlapBottom;
        float overlapTop;
        float overlapLeftSide;
        float overlapRightSide;
        bool hasCollision;

        CollisionInfo(float bottom = 0.f, float top = 0.f, float left = 0.f, float right = 0.f, bool collision = false)
        : overlapBottom(bottom), overlapTop(top), overlapLeftSide(left), overlapRightSide(right), hasCollision(collision) {}
    };

    enum class PlayerCondition
    {
        Normal,
        Protected
    };

    enum State
    {
        Moving = 1 << 0,
        Jumping = 1 << 1,
        Shooting = 1 << 2,
        Respawning = 1 << 3,
        Blinking = 1 << 4
    };

    enum class Direction
    {
        Left,
        Right
    };


private:

    // Dependency Injection
    std::shared_ptr<TextureLoader> txLoader;
    std::shared_ptr<AudioManager> audioManager;
    std::shared_ptr<HealthBar> healthBar;
    std::unique_ptr<ProjectilePool> projectilePool;

    //Movement
    sf::Vector2f velocity = sf::Vector2f(100.f, 10.f);
    float positionThresholdY;
    sf::Vector2f saveLastPos;
    sf::Vector2f respawnPos;
    sf::Vector2f previousPosition;
    sf::Vector2f maxPosition;
    sf::Vector2f position;
    const float horizontalVelocity = 100.0f;
    const float speed = 0.05f;
    const float gravity = 600.f;
    const float boundingBoxOffsetX = 18;
    const float boundingBoxOffsetY = 5;
    const float jumpVelocity = 280.f;

    //Animation
    sf::Sprite sprite;
    int currentAnim = 0;
    float scale = 0.f;
    sf::Clock respawnTimer;
    sf::Clock blinkingTimer;
    sf::Clock loseLifeCooldown;
    const float rebirth_animation_duration = 1.f;
    const float rebirth_animation_interval = 0.2f;
    const int maxFrames = 8;
    const float animationInterval = 0.1f;
    const float blinkingInterval = 0.1f;
    float animationTimer = 0.f;
    float rebornAnimationTimer = 0.f;
    const float rebirthVerticaloffset = 10.f;
    bool isRespawnTimerRestarted = false;
    bool isVisible = true;

    //Projectiles
    sf::Clock projectileResetTimer;
    int projectilesCount = 0;
    int maxProjectileCount = 0;
    float projectileAccumulatedTime = 0.f;
    float projectileResetDelay = 3.f;
    const float projectileResetInterval = 3.0f;
    const float projectileOffsetX = 20.f;
    const float projectileVelocity = 500.f;

    // Collision
    RayCast::Ray ray{sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 0.f)};
    sf::RectangleShape boundingBoxPlayer;
    bool collisionGround = false;
    bool collisionSide = false;
    bool collisionTop = false;

    // Health
    int health = 0;
    int maxHealth = 0;


    // Flags
    bool isShooting = false;
    bool coinsExchanged = false;
    int coinsCollected = 0;

    // States
    PlayerCondition condition;
    int state;

    // const variables
    const float epsilon = 3.0f;
    const float loseLifeDelay = 2.0f;
    const int tileSize = 32;
    const sf::Color normalColor = sf::Color(255,255,255);


public:

    // Constructor
    Player(const std::shared_ptr<TextureLoader> &txLoader, std::unique_ptr<ProjectilePool> projectilePool, std::shared_ptr<AudioManager>& audioManager);

    // Initialization
    void Initialize(const sf::Vector2f& position, const int maxHealth, const int projectilesAmount, const float scale, const float positionThresholdY);

    // Update
    void Update(const std::shared_ptr<RespawnManager>& respawnManager, const std::shared_ptr<Camera> &camera, const bool moveRight,const bool moveLeft, const bool shoot, const bool respawn, const bool exchangeCoins, const float dt, const std::vector<sf::RectangleShape> &tilesShapes, std::vector<sf::RectangleShape>& enemiesShapes, std::vector<sf::RectangleShape>& flyingEnemiesShapes, std::vector<sf::RectangleShape>& obstaclesShapes);
    void UpdateView(const bool moveRight, const bool moveLeft);

    // Movement
    void HandleFalling();
    void HandleHorizontalMovement(const std::shared_ptr<Camera> &camera, const float dt);
    void HandleVerticalMovement(const float dt);
    void Jump(const bool jumped, const float dt);
    void HandleShooting(const bool shoot, const float dt);
    Player::Direction CalculateDirection();

    // Collisions
    void HandleObstacleCollision();
    void HandleEnemyCollision();
    sf::RectangleShape CreateBoundingBox();
    void HandleGroundCollision(const sf::FloatRect &otherBounds, const float playerHalfHeight);
    void HandleTopCollision(const sf::FloatRect &otherBounds, const float playerHalfHeight);
    void HandleFlyingEnemyCollision();
    CollisionInfo CalculateCollision(const sf::FloatRect &playerBounds, const sf::FloatRect &otherBounds);
    bool CheckPlatformsCollision(const sf::FloatRect &playerBounds, const std::vector<sf::RectangleShape> &tiles, const float playerHalfHeight, const float playerHalfWidth);
    bool CheckEnemiesCollision(const sf::FloatRect &playerBounds, const std::vector<sf::RectangleShape> &enemies, const float playerHalfHeight, const float playerHalfWidth);
    bool CheckObstaclesCollisions(const sf::FloatRect &playerBounds, const std::vector<sf::RectangleShape> &obstacles, const float playerHalfHeight, const float playerHalfWidth);
    void CheckCollisionGround(const std::vector<sf::RectangleShape> &tiles, const std::vector<sf::RectangleShape> &enemies,std::vector<sf::RectangleShape> &flyingEnemiesShapes, const std::vector<sf::RectangleShape> &obstaclesShapes);
    bool CheckFlyingEnemiesCollision(const sf::FloatRect &playerBounds, std::vector<sf::RectangleShape> &flyingEnemiesShapes, const float playerHalfHeight, const float playerHalfWidth);
    void HandleLeftCollision(const sf::FloatRect &otherBounds, const float playerHalfWidth);
    void HandleRightCollision(const sf::FloatRect &otherBounds, const float playerHalfWidth);


    // Animation
    void CalculateCurrentAnimation(const float dt);
    void ResetAnimation(const int animYIndex);
    void HandleBlinking();
    void StartBlinking();
    void HandleRebirthAnimation();
    void HandleMovementAnimation();
    void HandleRespawn();

    // Health
    void IncreaseHealth(); 
    void DecreaseHealth(); 
    bool IsPlayerProtected();
    void LoseLife();

    //Getters
    sf::Vector2f GetPosition() const;
    sf::Vector2f GetMaxPosition() const;
    int GetProjectilesCount() const;
    std::vector<Projectile*> GetActiveProjectiles() const;
    sf::RectangleShape GetBoundingBox() const;
    sf::Vector2f GetVelocity() const;
    int GetCoins() const;
    bool IsInRebirth() const;
    int GetHealth() const; 
    int GetMaxHealth() const;

    // States
    void SetState(State stateToSet);
    void ClearState(State stateToClear);
    bool IfStateActive(State stateToCheck);
    void InputToState(bool moveRight, bool moveLeft, bool shoot, bool respawn);

    // Reset
    void ResetProjectilesCount();
    void ResetHealth();
    void ResetCoins();
    void HandleProjectileReset(const float dt);
    void ResetProjectiles();
    void ResetBlinking();
    void DecreaseCoins();


    void PickUpCoin();
    void IncreaseProjectiles();
    void HandleCoinLifeExchange(bool exchangeCoins);

    // Draw
    void Draw(const std::shared_ptr<sf::RenderTarget>& rt) const;
};
