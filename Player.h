#pragma once
#include <SFML/Graphics.hpp>

class Player
{
private:
    // view
    sf::Texture texture;
    sf::Sprite sprite;
    int currentAnim;
    // model
    float speed;
    sf::Vector2f velocity;
    float animationTimer;
    float animationInterval;
    sf::Vector2f position;
    float gravity;
    bool onGround;
    bool jumped;
    bool stopped;

public:
    void Initialize(const sf::Vector2f &pos);
    void Move(bool moveRight, bool moveLeft, float dt, float leftBound);
    void Draw(std::shared_ptr<sf::RenderTarget> rt) const;
    sf::Vector2f GetPosition() const;
    void UpdateView(bool moveRight, bool moveLeft);
    void Update(float dt);
    void Jump(bool jumped, float dt);
    void ResetAnimation(int animYIndex);
};