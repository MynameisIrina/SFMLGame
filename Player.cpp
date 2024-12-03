#include "Player.h"
#include <iostream>

void Player::Initialize(const sf::Vector2f &pos)
{
    speed = 0.05f;
    velocity = sf::Vector2f(0.f, 0.f);
    currentAnim = 0;
    animationTimer = 0.f;
    animationInterval = 0.1f;
    position = pos;

    if (texture.loadFromFile("SFMLGame/Assets/Player/Textures/Cat-Sheet.png"))
    {
        std::cout << "Player image loaded" << std::endl;
        sprite.setTexture(texture);

        // pick an image from the sprite
        int XIndex = 0;
        int YIndex = 4;
        sprite.setTextureRect(sf::IntRect(XIndex * 32, YIndex * 32, 32, 32));

        // change the origin of the sprite
        sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
        sprite.setScale(3, 3);
        sprite.setPosition(pos);

        gravity = 600.f;
        velocity = sf::Vector2f(0.01f, 0.05f);
        onGround = true;
        jumped = false;
        stopped = true;
    }
    else
    {
        std::cout << "Player image failed to load" << std::endl;
    }
}

void Player::Update(float dt)
{
    // calculate current sprite sheet image
    animationTimer += dt;
    if (animationTimer >= animationInterval)
    {
        currentAnim++;

        // if the player stopped moving, reset its animation
        if ((currentAnim >= 8 || stopped ) && onGround)
        {
            currentAnim = 0;
            ResetAnimation(4);
        }
        // if the player is jumping, play only one animation unit
        else if(currentAnim >= 0 && !onGround)
        {
            currentAnim = 0;
            ResetAnimation(41);
        }
        animationTimer = 0.f;
    }
}

void Player::ResetAnimation(int animYIndex)
{
    sprite.setTextureRect(sf::IntRect(0, animYIndex * 32, 32, 32));
}

void Player::Move(bool moveRight, bool moveLeft, float dt)
{

    if (moveRight)
    {
        position.x += velocity.x * dt;
        position.y += velocity.y * dt;
    }
    else if (moveLeft)
    {
        if (position.x - (32 / 2) > 0)
        {
            position.x -= velocity.x * dt;
            position.y -= velocity.y * dt;
        }
    }
}

void Player::UpdateView(bool moveRight, bool moveLeft)
{
    int YIndex = 4;

    if (moveRight)
    {
        stopped = false;
        sprite.setScale(1 * 3, 1 * 3);
        sprite.setTextureRect(sf::IntRect(currentAnim * 32, YIndex * 32, 32, 32));
    }
    else if (moveLeft)
    {
        stopped = false;
        sprite.setScale(-1 * 3, 1 * 3);
        sprite.setTextureRect(sf::IntRect(currentAnim * 32, YIndex * 32, 32, 32));
    }
    else
    {
        stopped = true;
    }

    sprite.setPosition(position);
}

void Player::Jump(bool jump, float dt)
{
    jumped = jump;
    if (jumped && onGround)
    {
        onGround = false;
        velocity.y = -200.f;
    }

    if (!onGround)
    {
        int YIndex = 41;
        sprite.setTextureRect(sf::IntRect(currentAnim * 32, YIndex * 32, 32, 32));

        velocity.y += gravity * dt;
        position.y += velocity.y * dt;
        // stop the player when it reaches ground
        if (position.y >= 502.f)
        {
            onGround = true;
            velocity.y = 0.0f;
            YIndex = 4;
            sprite.setTextureRect(sf::IntRect(currentAnim * 32, YIndex * 32, 32, 32));
        }
    }
}

void Player::Draw(sf::RenderTarget &rt) const
{
    rt.draw(sprite);
}

sf::Vector2f Player::GetPosition() const
{
    return position;
}
