#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "Math.h"
#include "TextureLoader.h"
#include "HealthBar.h"
#include "Tile.h"
#include "RayCast.h"
#include "ProjectilePool.h"
#include "AudioManager.h"

class Enemy;
class RespawnManager;

class Player
{
public:

    struct CollisionInfo
    {
        float overlapBottom;
        float overlapTop;
        bool hasCollision;

        CollisionInfo(float bottom = 0.f, float top = 0.f, bool collision = false)
        : overlapBottom(bottom), overlapTop(top), hasCollision(collision) {}
    };

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
        Blinking = 1 << 4,
        Idle = 1 << 5
    };


private:

    // Private data
    std::shared_ptr<TextureLoader> txLoader;
    std::shared_ptr<AudioManager> audioManager;
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
    float scale = 0.f;
    PlayerCondition condition;
    sf::Clock loseLifeCooldown;
    sf::Clock respawnTimer;
    sf::Clock blinkingTimer;
    sf::Clock projectileResetTimer;
    PlayerCondition currentState;
    ProjectilePool &projectilePool;
    int state;
    float positionThresholdY;
    int projectilesCount = 0;
    int maxProjectileCount = 0;
    int coinsCollected = 0;
    sf::Vector2f previousPosition;
    float projectileAccumulatedTime = 0.f;
    float projectileResetDelay = 3.f;


    // const variables
    const float epsilon = 3.0f;
    const float horizontalVelocity = 100.0f;
    const float speed = 0.05f;
    const float gravity = 600.f;
    const float rebirth_animation_duration = 1.f;
    const float rebirth_animation_interval = 0.2f;
    const int maxFrames = 8;
    const float animationInterval = 0.1f;
    const float loseLifeDelay = 2.0f;
    const float blinkingInterval = 0.1f;
    const float projectileResetInterval = 3.0f;
    const int tileSize = 32;
    const int boundingBoxOffsetX = 15;
    const int boundingBoxOffsetY = 5;
    const float projectileOffsetX = 10.f;
    const float projectileVelocity = 500.f;
    const float jumpVelocity = 280.f;
    const float rebirthVerticaloffset = 10.f;
    const sf::Color normalColor = sf::Color(255,255,255);

    // Flags
    bool collisionGround = false;
    bool collisionSide = false;
    bool collisionTop = false;
    bool collisionObstacle = false;
    bool isRespawnTimerRestarted = false;
    bool isVisible = true;
    bool isShooting = false;
    bool coinsExchanged = false;

    // Animation
    float animationTimer = 0.f;
    float rebornAnimationTimer = 0.f;

    // Health
    int health = 0;
    int maxHealth = 0;

public:
    // Constructor
    Player(const std::shared_ptr<TextureLoader> &txLoader, ProjectilePool &projectilePool, std::shared_ptr<AudioManager>& audioManager);

    // Initialization
    void Initialize(const sf::Vector2f& position, const int maxHealth, const int projectilesAmount, const float scale, const float positionThresholdY);

    // Update
    void Update(const std::shared_ptr<RespawnManager>& respawnManager, const std::shared_ptr<Camera> &camera, const bool moveRight,const bool moveLeft, const bool shoot, const float leftBound, const bool respawn, const bool exchangeCoins, const float dt, const std::vector<sf::RectangleShape> &tilesShapes, std::vector<sf::RectangleShape>& enemiesShapes, std::vector<sf::RectangleShape>& obstaclesShapes);
    void UpdateView(const bool moveRight, const bool moveLeft);

    // Movement
    void HandleFalling();
    void HandleHorizontalMovement(const float dt, const float leftBound);
    void HandleVerticalMovement(const float dt);
    void Jump(const bool jumped, const float dt);
    void HandleShooting(const bool shoot, const float dt);
    int CalculateDirection();

    // Collisions
    //void CheckCollisionGround(const std::vector<sf::RectangleShape> &tiles, std::vector<sf::RectangleShape>& enemies, std::vector<sf::RectangleShape>& obstaclesShapes);
    void CheckCollisionSide(const std::vector<sf::RectangleShape> &tiles, std::vector<sf::RectangleShape>& enemies, std::vector<sf::RectangleShape>& obstaclesShapes);
    void HandleObstacleCollision();
    void HandleEnemyCollision();
    sf::RectangleShape CreateBoundingBox();
    void HandleGroundCollision(const sf::FloatRect &otherBounds, const float playerHalfHeight);
    void HandleTopCollision(const sf::FloatRect &otherBounds, const float playerHalfHeight);
    CollisionInfo CalculateCollision(const sf::FloatRect &playerBounds, const sf::FloatRect &otherBounds);
    bool CheckPlatformsCollision(const sf::FloatRect &playerBounds, const std::vector<sf::RectangleShape> &tiles, const float playerHalfHeight);
    bool CheckEnemiesCollision(const sf::FloatRect &playerBounds, const std::vector<sf::RectangleShape> &enemies, const float playerHalfHeight);
    bool CheckObstaclesCollisions(const sf::FloatRect &playerBounds, const std::vector<sf::RectangleShape> &obstacles, const float playerHalfHeight);
    void CheckCollisionGround(const std::vector<sf::RectangleShape> &tiles, const std::vector<sf::RectangleShape> &enemies, const std::vector<sf::RectangleShape> &obstaclesShapes);



    // Animation
    void CalculateCurrentAnimation(const float dt);
    void ResetAnimation(const int animYIndex);
    void HandleBlinking();
    void HandleRebirthAnimation();
    void HandleMovementAnimation();

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
    void Draw(const std::shared_ptr<sf::RenderTarget>& rt) const;
    sf::Vector2f GetPosition() const;
    sf::Vector2f GetMaxPosition() const;
    int GetProjectilesCount() const;
    std::vector<Projectile*> GetActiveProjectiles() const;
    sf::RectangleShape GetBoundingBox() const;
    void ResetProjectilesCount();
    void PickUpCoin();
    int GetCoins();
    bool IsInRebirth();
    void ResetHealth();
    void ResetCoins();
    void ResetProjectiles();
    void ResetBlinking();
    void HandleProjectileReset(const float dt);
    void IncreaseProjectiles();
    void DecreaseCoins();
    void HandleCoinLifeExchange(bool exchangeCoins);

    // Utilities
    static void DrawRay(const std::shared_ptr<sf::RenderTarget> &rt, const sf::Vector2f& start, const sf::Vector2f& end, sf::Color color = sf::Color::Red);

    // States
    void SetState(State stateToSet);
    void ClearState(State stateToClear);
    bool IfStateActive(State stateToCheck);
    void InputToState(bool moveRight, bool moveLeft, bool shoot, bool respawn);
};
