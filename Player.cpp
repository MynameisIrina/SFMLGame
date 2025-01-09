#include "Player.h"
#include <iostream>

Player::Player(std::shared_ptr<TextureLoader> txLoaderRef): txLoader(txLoaderRef)
{}

void Player::Initialize(const sf::Vector2f &pos)
{
    position = pos;
    
    boundingRec.setSize(sf::Vector2f(30, 40));
    boundingRec.setFillColor(sf::Color::Transparent);
    boundingRec.setOutlineColor(sf::Color::Red);
    boundingRec.setOutlineThickness(1);

    sprite = txLoader->SetSprite(TextureLoader::TextureType::Player);
    
    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
    sprite.setScale(AnimationPlayer::SCALE, AnimationPlayer::SCALE);
    sprite.setPosition(pos);
}

void Player::Update(float dt, const std::vector<sf::RectangleShape> &tiles)
{
    CalculateCurrAnimation(dt);
    CheckCollision(tiles);
}

void Player::ResetAnimation(int animYIndex)
{
    sprite.setTextureRect(sf::IntRect(0, animYIndex * AnimationPlayer::TILE_SIZE, AnimationPlayer::TILE_SIZE, AnimationPlayer::TILE_SIZE));
}

void Player::CheckCollision(const std::vector<sf::RectangleShape> &tiles)
{
    collisionGround = false;
    collisionSide = false;

    for (const auto &tile : tiles)
    {
        sf::FloatRect playerBounds = boundingRec.getGlobalBounds();
        sf::FloatRect tileBounds = tile.getGlobalBounds();

        if (Math::CheckRectCollision(boundingRec.getGlobalBounds(), tile.getGlobalBounds()))
        {
            float overlapTop = playerBounds.top - (tileBounds.top + tileBounds.height);
            float overlapBottom = tileBounds.top - (playerBounds.top + playerBounds.height);

            if (overlapBottom <= 0.f && overlapBottom >= -1.f)
            {
                collisionGround = true;
                continue;
            }

            float overlapRight = tileBounds.left - (playerBounds.left + playerBounds.width);
            float overlapLeft = playerBounds.left - (tileBounds.left + tileBounds.width);

            if (abs(overlapRight) < abs(overlapLeft) && overlapRight <= 0.f && sprite.getScale().x > 0)
            {
                collisionSide = true;
            }
            else if (abs(overlapRight) > abs(overlapLeft) && overlapLeft <= 0.f && sprite.getScale().x < 0)
            {
                collisionSide = true;
            }
        }
    }
}

void Player::CalculateCurrAnimation(float dt)
{
    // calculate current sprite sheet image
    animationTimer += dt;
    if (animationTimer >= animationInterval)
    {
        currentAnim++;

        // if the player stopped moving, reset its animation
        if ((currentAnim >= 8 || stopped) && landedAfterJump)
        {
            currentAnim = 0;
            ResetAnimation(AnimationPlayer::STOP_MOVING);
        }
        // if the player is jumping, play only one animation unit
        else if (currentAnim >= 0 && !landedAfterJump)
        {
            currentAnim = 0;
            ResetAnimation(AnimationPlayer::STOPP_JUMPING);
        }
        animationTimer = 0.f;
    }
}

void Player::Move(bool moveRight, bool moveLeft, float dt, float leftBound)
{
    float offset = 27.f;

    if (collisionGround)
    {
        velocity.y = 0;
    }

    if (collisionSide)
    {
        velocity.x = 0;
    }

    if (collisionGround && !collisionSide)
    {
        velocity.x = 100.f;
    }

    if (!collisionGround && landedAfterJump)
    {
        velocity.y += gravity * dt;
        position.y += velocity.y * dt;
    }

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
    boundingRec.setPosition(sprite.getPosition().x - 16, sprite.getPosition().y + 10);
}

void Player::Jump(bool jump, float dt)
{
    jumped = jump;

    if (jumped && landedAfterJump)
    {
        landedAfterJump = false;
        velocity.y = -250.f;
    }

    if (!landedAfterJump)
    {
        sprite.setTextureRect(sf::IntRect(currentAnim * AnimationPlayer::TILE_SIZE, AnimationPlayer::JUMPING_Y * AnimationPlayer::TILE_SIZE, AnimationPlayer::TILE_SIZE, AnimationPlayer::TILE_SIZE));
        velocity.y += gravity * dt;
        position.y += velocity.y * dt;

        if (collisionGround)
        {
            landedAfterJump = true;
        }
    }
}

void Player::Draw(std::shared_ptr<sf::RenderTarget> rt) const
{
    rt->draw(sprite);
    rt->draw(boundingRec);
}

sf::Vector2f Player::GetPosition() const
{
    return position;
}
