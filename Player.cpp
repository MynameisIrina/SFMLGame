#include "Player.h"
#include <iostream>

void Player::Initialize(const sf::Vector2f &pos)
{
    position = pos;
    boundingRec.setSize(sf::Vector2f(64, 40));
    boundingRec.setFillColor(sf::Color::Transparent);
    boundingRec.setOutlineColor(sf::Color::Red);
    boundingRec.setOutlineThickness(1);

    if (texture.loadFromFile("SFMLGame/Assets/Player/Textures/Cat-Sheet.png"))
    {
        std::cout << "Player image loaded" << std::endl;
        sprite.setTexture(texture);

        sprite.setTextureRect(sf::IntRect(AnimationPlayer::MOVING_X * AnimationPlayer::TILE_SIZE, AnimationPlayer::MOVING_Y * AnimationPlayer::TILE_SIZE, AnimationPlayer::TILE_SIZE, AnimationPlayer::TILE_SIZE));

        sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
        sprite.setScale(AnimationPlayer::SCALE, AnimationPlayer::SCALE);
        sprite.setPosition(pos);
    }
    else
    {
        std::cout << "Player image failed to load" << std::endl;
    }
}

void Player::Update(float dt, std::vector<sf::RectangleShape> tiles)
{
    CalculateCurrAnimation(dt);
    CheckCollision(tiles);

}

void Player::ResetAnimation(int animYIndex)
{
    sprite.setTextureRect(sf::IntRect(0, animYIndex * AnimationPlayer::TILE_SIZE, AnimationPlayer::TILE_SIZE, AnimationPlayer::TILE_SIZE));
}

void Player::CheckCollision(std::vector<sf::RectangleShape> tiles)
{
    onGround = false;

    for (const auto& tile : tiles)
    {
        if (Math::CheckRectCollision(boundingRec.getGlobalBounds(), tile.getGlobalBounds()))
        {
            sf::FloatRect playerBounds = boundingRec.getGlobalBounds();
            sf::FloatRect tileBounds = tile.getGlobalBounds();

            float overlapBottom = tileBounds.top - (playerBounds.top + playerBounds.height);
            float overlapTop = playerBounds.top - (tileBounds.top + tileBounds.height);

            if (overlapBottom <= 0 && velocity.y > 0) // Collision below
            {
                onGround = true;
                velocity.y = 0;
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

void Player::Move(bool moveRight, bool moveLeft, float dt, float leftBound)
{
    float offset = 27.f;

    // Apply gravity
    if (!onGround) velocity.y += collisionGravityFactor * dt;

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
    boundingRec.setPosition(sprite.getPosition().x - 32, sprite.getPosition().y + 10);
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
