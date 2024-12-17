#include "Player.h"
#include "Animation.h"
#include <iostream>

void Player::Initialize(const sf::Vector2f &pos)
{
    speed = 0.05f;
    velocity = sf::Vector2f(0.f, 0.f);
    animationTimer = 0.f;
    animationInterval = 0.1f;
    position = pos;
    gravity = 600.f;
    velocity = sf::Vector2f(100.f, 10.f);
    onGround = true;
    jumped = false;
    stopped = true;


    if (texture.loadFromFile("SFMLGame/Assets/Player/Textures/Cat-Sheet.png"))
    {
        std::cout << "Player image loaded" << std::endl;
        sprite.setTexture(texture);

        // pick an image from the sprite
        sprite.setTextureRect(sf::IntRect(AnimationPlayer::MOVING_X * AnimationPlayer::TILE_SIZE, AnimationPlayer::MOVING_Y * AnimationPlayer::TILE_SIZE, AnimationPlayer::TILE_SIZE, AnimationPlayer::TILE_SIZE));

        // change the origin of the sprite
        sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
        sprite.setScale(AnimationPlayer::SCALE, AnimationPlayer::SCALE);
        sprite.setPosition(pos);
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
        if ((currentAnim >= 8 || stopped) && onGround)
        {
            currentAnim = 0;
            ResetAnimation(AnimationPlayer::STOP_MOVING);
        }
        // if the player is jumping, play only one animation unit
        else if (currentAnim >= 0 && !onGround)
        {
            currentAnim = 0;
            ResetAnimation(AnimationPlayer::STOPP_JUMPING);
        }
        animationTimer = 0.f;
    }
}

void Player::ResetAnimation(int animYIndex)
{
    sprite.setTextureRect(sf::IntRect(0, animYIndex * AnimationPlayer::TILE_SIZE, AnimationPlayer::TILE_SIZE, AnimationPlayer::TILE_SIZE));
}


void Player::Move(bool moveRight, bool moveLeft, float dt, float leftBound)
{
    float offset = 27.f;

    if (moveRight)
    {
        stopped = false;
        position.x += velocity.x * dt;
    }
    else if (moveLeft)
    {
        stopped = false;
        if (position.x > leftBound + offset)
        {
            position.x -= velocity.x * dt;
        }
    }
    else
    {
        stopped = true;
    }
}

void Player::UpdateView(bool moveRight, bool moveLeft)
{
    if (moveRight)
    {
        sprite.setScale(AnimationPlayer::SCALE, AnimationPlayer::SCALE);
        sprite.setTextureRect(sf::IntRect(currentAnim * AnimationPlayer::TILE_SIZE, AnimationPlayer::MOVING_Y * AnimationPlayer::TILE_SIZE, AnimationPlayer::TILE_SIZE, AnimationPlayer::TILE_SIZE));
    }
    else if (moveLeft)
    {
        sprite.setScale(-1 * AnimationPlayer::SCALE, AnimationPlayer::SCALE);
        sprite.setTextureRect(sf::IntRect(currentAnim * AnimationPlayer::TILE_SIZE, AnimationPlayer::MOVING_Y * AnimationPlayer::TILE_SIZE, AnimationPlayer::TILE_SIZE, AnimationPlayer::TILE_SIZE));
    }
    
    sprite.setPosition(position);
}

void Player::Jump(bool jump, float dt)
{
    jumped = jump;
    if (jumped && onGround)
    {
        onGround = false;
        velocity.y = -250.f;
    }

    if (!onGround)
    {
        sprite.setTextureRect(sf::IntRect(currentAnim * AnimationPlayer::TILE_SIZE, AnimationPlayer::JUMPING_Y * AnimationPlayer::TILE_SIZE, AnimationPlayer::TILE_SIZE, AnimationPlayer::TILE_SIZE));

        velocity.y += gravity * dt;
        position.y += velocity.y * dt;
        // stop the player when it reaches ground
        if (position.y >= 502.f)
        {
            onGround = true;
            velocity.y = 0.0f;
            sprite.setTextureRect(sf::IntRect(currentAnim * AnimationPlayer::TILE_SIZE, AnimationPlayer::MOVING_Y * AnimationPlayer::TILE_SIZE, AnimationPlayer::TILE_SIZE, AnimationPlayer::TILE_SIZE));
        }
    }
}



void Player::Draw(std::shared_ptr<sf::RenderTarget> rt) const
{
    rt->draw(sprite);
}

sf::Vector2f Player::GetPosition() const
{
    return position;
}
