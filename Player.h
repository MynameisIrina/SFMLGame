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

    enum MotionState
    {
        Moving,
        Stopped
    };

private:
    // Private data
    static std::shared_ptr<TextureLoader> txLoader;
    sf::Sprite sprite;
    std::shared_ptr<HealthBar> healthBar;
    int currentAnim = 0;
    sf::RectangleShape boundingRecPlayer;
    RayCast::Ray ray{sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 0.f)};
    sf::Vector2f velocity = sf::Vector2f(100.f, 10.f);
    sf::Vector2f saveLastPos = sf::Vector2f(35.f, 20.f);
    sf::Vector2f respawnPos = sf::Vector2f(35.f, 20.f);
    sf::Vector2f maxPosition = sf::Vector2f(35.f, 20.f);
    sf::Vector2f position;
    PlayerCondition condition;
    sf::Clock loseLifeCooldown;

    // Static constants
    static constexpr float epsilon = 3.0f;
    static constexpr float horizontalVelocity = 100.0f;
    static constexpr float speed = 0.05f;
    static constexpr float gravity = 600.f;
    static constexpr float animationInterval = 0.1f;
    static constexpr float loseLifeDelay = 1.0f;

    // Flags
    bool isJumping = false;
    bool collisionGround = false;
    bool collisionSide = false;
    bool collisionTop = false;
    bool stopped = true;
    bool respawn = false;
    bool moveRight;
    bool moveLeft;
    bool atRespawnPos = false;
    bool collisionObstacle = false;

    // Animation
    float animationTimer = 0.f;

    // Health
    int health = 0; 
    int maxHealth = 0;

public:
    // Constructor
    Player(std::shared_ptr<TextureLoader> txLoaderRef);

    // Initialization
    void Initialize(const sf::Vector2f &pos, int maxHealthRef);

    // Update
    void Update(bool moveRight, bool moveLeft, float leftBound, bool respawn, float dt, std::vector<Tile> &boundRecs);
    void UpdateView(bool moveRight, bool moveLeft);

    // Movement
    void HandleFalling();
    void HandleHorizontalMovement(float dt, float leftBound);
    void HandleVerticalMovement(float dt);
    void Jump(bool jumped, float dt);

    // Collisions
    void CheckCollisionGround(std::vector<Tile> &platforms);
    void CheckCollisionSide(std::vector<Tile> &platforms);

    // Animation
    void CalculateCurrAnimation(float dt);
    void ResetAnimation(int animYIndex);

    // Respawn
    void Respawn();

    // Health
    void IncreaseHealth();  // Increase health
    void DecreaseHealth();  // Decrease health
    int GetHealth() const;  // Get current health
    int GetMaxHealth() const;  // Get max health
    bool IsPlayerProtected();  // Check if the player is protected

    // Other
    void Draw(const std::shared_ptr<sf::RenderTarget> rt);
    sf::Vector2f GetPosition() const;
    sf::Vector2f GetMaxPosition() const;
    bool IsRespawn() const;
    bool IsMoveLeft() const;

    // Utilities
    static void DrawRay(std::shared_ptr<sf::RenderTarget> window, const sf::Vector2f &start, const sf::Vector2f &end, sf::Color color = sf::Color::Red);

    // Health management methods
    void LoseLife();   // Lose a life
    void GainLife();   // Gain a life
};
