#include "Player.h"
#include <iostream>

std::shared_ptr<TextureLoader> Player::txLoader = nullptr;

Player::Player(std::shared_ptr<TextureLoader> txLoaderRef)
{
    txLoader = txLoaderRef;
}

void Player::Initialize(const sf::Vector2f &pos, int maxHealthRef)
{
    position = pos;
    maxHealth = maxHealthRef;
    health = maxHealth;
    condition = Normal;

    boundingRecPlayer.setSize(sf::Vector2f(30, 40));
    boundingRecPlayer.setFillColor(sf::Color::Transparent);
    boundingRecPlayer.setOutlineColor(sf::Color::Red);
    boundingRecPlayer.setOutlineThickness(1);

    sprite = txLoader->SetSprite(TextureLoader::TextureType::Player);

    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
    sprite.setScale(AnimationPlayer::SCALE, AnimationPlayer::SCALE);
    sprite.setPosition(pos);
}

void Player::Update(bool moveRight, bool moveLeft, float leftBound, bool respawn, float dt, std::vector<Tile> &boundRecs)
{
    this->respawn = respawn;
    this->moveLeft = moveLeft;
    this->moveRight = moveRight;

    CalculateCurrAnimation(dt);
    HandleFalling();
    HandleVerticalMovement(dt);
    HandleHorizontalMovement(dt, leftBound);
    CheckCollisionGround(boundRecs);
    CheckCollisionSide(boundRecs);
}

void Player::HandleFalling()
{
    if (!IsPlayerProtected() && position.y > 800)
    {
        DecreaseHealth();
        Respawn();
        loseLifeCooldown.restart();
    }
}

void Player::HandleHorizontalMovement(float dt, float leftBound)
{
    if (moveRight)
    {
        stopped = false;
        atRespawnPos = false;
        position.x += velocity.x * dt;
    }
    else if (moveLeft)
    {
        stopped = false;
        atRespawnPos = false;
        if (position.x > leftBound + 27.f)
        {
            position.x -= velocity.x * dt;
        }
    }
    else
    {
        stopped = true;
    }
}

void Player::HandleVerticalMovement(float dt)
{
    if (collisionTop)
    {
        velocity.y = 0;
    }

    if (!collisionGround)
    {
        velocity.y += gravity * dt;
        collisionTop = false;
    }
    else
    {
        velocity.y = 0;
    }

    if ((collisionGround && !collisionSide) || (!collisionGround && !collisionSide))
    {
        velocity.x = horizontalVelocity;
    }

    position.y += velocity.y * dt;
}

void Player::CheckCollisionSide(std::vector<Tile> &platforms)
{
    collisionSide = false;

    bool isMovingRight = sprite.getScale().x > 0;
    float direction = isMovingRight ? 1.0f : -1.0f;

    sf::Vector2f rayMiddle_Start = boundingRecPlayer.getPosition() + sf::Vector2f(boundingRecPlayer.getSize().x / 2, boundingRecPlayer.getSize().y / 2);
    sf::Vector2f rayMiddle_End = rayMiddle_Start + sf::Vector2f(direction * boundingRecPlayer.getSize().x, 0);

    ray = RayCast::Ray(rayMiddle_Start, rayMiddle_End);

    auto hit = RayCast::DoRaycast(rayMiddle_Start, rayMiddle_End, platforms);

    if (hit)
    {

        RayCast::RayCastHit hitInfo = hit.value();

        if (!IsPlayerProtected() && hitInfo.typeTile == Tile::Obstacle)
        {
            DecreaseHealth();
            loseLifeCooldown.restart();
        }

        if (isMovingRight)
        {
            position.x = hitInfo.hitRect.getGlobalBounds().left - boundingRecPlayer.getGlobalBounds().width + epsilon;
        }
        else
        {
            position.x = hitInfo.hitRect.getGlobalBounds().left + hitInfo.hitRect.getGlobalBounds().width + boundingRecPlayer.getGlobalBounds().width - epsilon;
        }

        collisionSide = true;
        velocity.x = 0;
    }
}

void Player::Jump(bool jump, float dt)
{
    if (jump && !isJumping)
    {
        isJumping = true;
        collisionGround = false;
        velocity.y = -280.f;
    }
}

void Player::ResetAnimation(int animYIndex)
{
    sprite.setTextureRect(sf::IntRect(0, animYIndex * AnimationPlayer::TILE_SIZE, AnimationPlayer::TILE_SIZE, AnimationPlayer::TILE_SIZE));
}

void Player::CheckCollisionGround(std::vector<Tile> &platforms)
{
    collisionGround = false;
    collisionTop = false;

    for (auto &platform : platforms)
    {
        Tile::TileData tileData = platform.tileData;
        sf::FloatRect playerBounds = boundingRecPlayer.getGlobalBounds();
        sf::FloatRect tileBounds = tileData.shape.getGlobalBounds();

        if (Math::CheckRectCollision(playerBounds, tileBounds))
        {
            if (!IsPlayerProtected() && tileData.type == Tile::Obstacle)
            {
                DecreaseHealth();
                loseLifeCooldown.restart();
            }

            platform.tileData.shape.setOutlineColor(sf::Color::Green);

            float overlapTop = playerBounds.top - (tileBounds.top + tileBounds.height);
            float overlapBottom = tileBounds.top - (playerBounds.top + playerBounds.height);

            if (overlapBottom <= 0.f && overlapBottom >= -5.f)
            {
                collisionGround = true;
                isJumping = false;
                continue;
            }
            if (overlapTop <= 0.f && overlapTop >= -5.f)
            {
                collisionTop = true;
                continue;
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
        if ((currentAnim >= 8 || stopped) && !isJumping)
        {
            currentAnim = 0;
            ResetAnimation(AnimationPlayer::STOP_MOVING);
        }
        // if the player is jumping, play only one animation unit
        else if (currentAnim >= 0 && isJumping)
        {
            currentAnim = 0;
            ResetAnimation(AnimationPlayer::STOPP_JUMPING);
        }
        animationTimer = 0.f;
    }
}

void Player::Respawn()
{
    if (!atRespawnPos)
    {

        if (position.x > maxPosition.x)
        {
            maxPosition = position;
        }

        respawn = true;
        position = respawnPos;
        velocity.y = 0;
        sprite.setScale(AnimationPlayer::SCALE, AnimationPlayer::SCALE);
        atRespawnPos = true;
    }
    else
    {
        std::cout << "Already at respawn point" << std::endl;
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
    boundingRecPlayer.setPosition(sprite.getPosition().x - 16, sprite.getPosition().y + 10);
}

void Player::Draw(const std::shared_ptr<sf::RenderTarget> rt)
{
    rt->draw(sprite);
    rt->draw(boundingRecPlayer);
    DrawRay(rt, ray.start, ray.end);
}

void Player::DrawRay(std::shared_ptr<sf::RenderTarget> window, const sf::Vector2f &start, const sf::Vector2f &end, sf::Color color)
{
    sf::VertexArray ray(sf::Lines, 2);
    ray[0].position = start;
    ray[0].color = color;
    ray[1].position = end;
    ray[1].color = color;

    window->draw(ray);
}

void Player::IncreaseHealth()
{
    if (health < maxHealth)
    {
        health++;
    }
}

void Player::DecreaseHealth()
{
    if (health > 0)
    {
        health--;
    }
}

int Player::GetHealth() const
{
    return health;
}

int Player::GetMaxHealth() const
{
    return maxHealth;
}

bool Player::IsPlayerProtected()
{
    if (loseLifeCooldown.getElapsedTime().asSeconds() >= loseLifeDelay)
    {
        condition = Normal;
        return false;
    }

    condition = Protected;
    return true;
}

bool Player::IsRespawn() const
{
    return respawn;
}

bool Player::IsMoveLeft() const
{
    return moveLeft;
}

sf::Vector2f Player::GetMaxPosition() const
{
    return maxPosition;
}

sf::Vector2f Player::GetPosition() const
{
    return position;
}