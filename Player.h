#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "Math.h"
#include "TextureLoader.h"

class Player
{
private:
    // view
    std::shared_ptr<TextureLoader> txLoader;
    sf::Sprite sprite;

    int currentAnim = 0;
    sf::RectangleShape boundingRecPlayer;
    // model
    float speed = 0.05f;
    sf::Vector2f velocity = sf::Vector2f(100.f, 10.f);;
    float animationTimer = 0.f;
    float animationInterval = 0.1f;
    sf::Vector2f position;
    float gravity = 600.f;
    float collisionGravityFactor = 0.1f;
    bool isJumping = false;
    bool collisionGround = false;
    bool collisionSide = false;
    bool collisionTop = false;
    bool landed = false;
    bool jumped = false;
    bool stopped = true;
    bool respawn = false;
    bool atRespawnPos = false;
    sf::Vector2f saveLastPos = sf::Vector2f(35.f, 20.f);
    sf::Vector2f respawnPos = sf::Vector2f(35.f, 20.f);
    Math math;
    bool collisionObstacle = false;

public:
    Player(std::shared_ptr<TextureLoader> txLoaderRef);
    void Initialize(const sf::Vector2f &pos);
    void Move(bool moveRight, bool moveLeft, float dt, float leftBound);
    void Draw(const std::shared_ptr<sf::RenderTarget> rt);
    sf::Vector2f GetPosition() const;
    void UpdateView(bool moveRight, bool moveLeft);
    void Update(bool moveRight, bool moveLeft, float leftBound, float dt, std::vector<sf::RectangleShape> &boundRecs);
    void Jump(bool jumped, float dt);
    void ResetAnimation(int animYIndex);
    void CheckCollisionGround(std::vector<sf::RectangleShape> &boundRecs);
    void CheckCollisionSide(std::vector<sf::RectangleShape> &boundRecs);
    void CalculateCurrAnimation(float dt);
    void Respawn();
    sf::Vector2f GetLastSavedPos();
    bool castRay(const sf::RectangleShape& player, std::vector<sf::RectangleShape>& platforms, float rayLength, bool isMovingRight);
    void drawRay(std::shared_ptr<sf::RenderTarget> window, const sf::Vector2f& start, const sf::Vector2f& end, sf::Color color = sf::Color::Red);
    void Debug(std::shared_ptr<sf::RenderTarget> window, std::vector<sf::RectangleShape> &boundRecs, const sf::Vector2f& start, const sf::Vector2f& end, sf::Color color = sf::Color::Red);
};