#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "Math.h"
#include "TextureLoader.h"
#include "HealthBar.h"
#include "Tile.h"
#include "RayCast.h"

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

    // const variables
    const float epsilon = 3.0f;
    const float horizontalVelocity = 100.0f;
    const float speed = 0.05f;
    const float gravity = 600.f;
    const float rebirth_animation_duration = 0.8f;
    const float rebirth_animation_interval = 0.1f;
    const int max_frames = 8;
    const float animationInterval = 0.1f;
    const float loseLifeDelay = 2.0f;
    const float blinkingInterval = 0.1f;

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

    // Animation
    float animationTimer = 0.f;
    float rebornAnimationTimer = 0.f;

    // Health
    int health = 0;
    int maxHealth = 0;

public:
    // Constructor
    Player(const std::shared_ptr<TextureLoader>& txLoader);

    // Initialization
    void Initialize(sf::Vector2f pos, int maxHealthRef, float scale);

    // Update
    void Update(bool moveRight, bool moveLeft, float leftBound, bool respawn, float dt, std::vector<Tile> &tiles);
    void UpdateView(bool moveRight, bool moveLeft);

    // Movement
    void HandleFalling();
    void HandleHorizontalMovement(float dt, float leftBound);
    void HandleVerticalMovement(float dt);
    void Jump(bool jumped, float dt);

    // Collisions
    void CheckCollisionGround(const std::vector<Tile> &tiles);
    void CheckCollisionSide(const std::vector<Tile> &tiles);
    void HandleObstacleCollision();
    sf::RectangleShape CreateBoundingBox();

    // Animation
    void CalculateCurrAnimation(float dt);
    void ResetAnimation(int animYIndex);
    void HandleBlinking();

    // Respawn
    void Respawn();
    void HandleRespawn(bool respawn);

    // Health
    void IncreaseHealth();    // Increase health
    void DecreaseHealth();    // Decrease health
    int GetHealth() const;    // Get current health
    int GetMaxHealth() const; // Get max health
    bool IsPlayerProtected(); // Check if the player is protected

    // Other
    void Draw(const std::shared_ptr<sf::RenderTarget> rt);
    sf::Vector2f GetPosition() const;
    sf::Vector2f GetMaxPosition() const;
    sf::RectangleShape GetBoundingBox();
    bool IsRespawn() const;
    bool IsMoveLeft() const;

    // Utilities
    static void DrawRay(const std::shared_ptr<sf::RenderTarget>& rt, const sf::Vector2f start, const sf::Vector2f end, sf::Color color = sf::Color::Red);

    // Health management methods
    void LoseLife(); // Lose a life
    void GainLife(); // Gain a life
};
