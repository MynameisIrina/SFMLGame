#include "Player.h"
#include <iostream>

Player::Player(const std::shared_ptr<TextureLoader> &txLoader) : txLoader(txLoader) {}

void Player::Initialize(sf::Vector2f position, int maxHealth, float scale)
{
    this->position = position;
    this->maxHealth = maxHealth;
    this->scale = scale;
    health = maxHealth;
    condition = Normal;

    sprite = txLoader->SetSprite(TextureLoader::TextureType::Player);

    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
    sprite.setScale(scale, scale);
    sprite.setPosition(position);

    boundingBoxPlayer.setSize(sf::Vector2f(sprite.getLocalBounds().width, sprite.getLocalBounds().height));
    boundingBoxPlayer.setFillColor(sf::Color::Transparent);
    boundingBoxPlayer.setOutlineColor(sf::Color::Red);
    boundingBoxPlayer.setOutlineThickness(1);
}

void Player::Update(bool moveRight, bool moveLeft, float leftBound, bool respawn, float dt, std::vector<Tile> &tiles)
{
    this->respawn = respawn;
    this->moveLeft = moveLeft;
    this->moveRight = moveRight;

    HandleRespawn(respawn);
    HandleFalling();
    CalculateCurrAnimation(dt);
    HandleVerticalMovement(dt);
    HandleHorizontalMovement(dt, leftBound);
    CheckCollisionGround(tiles);
    CheckCollisionSide(tiles);
    AddBlinking();
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
        atRespawnPos = false;
        position.x += velocity.x * dt;
    }
    else if (moveLeft)
    {
        atRespawnPos = false;
        if (position.x > leftBound + 27.f)
        {
            position.x -= velocity.x * dt;
        }
    }

    if (collisionSide)
    {
        velocity.x = 0;
    }

    if ((collisionGround && !collisionSide) || (!collisionGround && !collisionSide))
    {
        velocity.x = horizontalVelocity;
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

    position.y += velocity.y * dt;
}

void Player::CheckCollisionSide(const std::vector<Tile> &tiles)
{
    collisionSide = false;

    bool isMovingRight = sprite.getScale().x > 0;
    float direction = isMovingRight ? 1.0f : -1.0f;

    sf::Vector2f rayMiddle_Start = boundingBoxPlayer.getPosition() + sf::Vector2f(boundingBoxPlayer.getSize().x / 2, boundingBoxPlayer.getSize().y / 2);
    sf::Vector2f rayMiddle_End = rayMiddle_Start + sf::Vector2f(direction * boundingBoxPlayer.getSize().x / 2 + epsilon, 0);

    ray = RayCast::Ray(rayMiddle_Start, rayMiddle_End);

    auto hit = RayCast::DoRaycast(rayMiddle_Start, rayMiddle_End, tiles);

    if (hit)
    {

        RayCast::RayCastHit hitInfo = hit.value();

        if (!IsPlayerProtected() && hitInfo.typeTile == Tile::Obstacle)
        {
            HandleObstacleCollision();
        }

        if (isMovingRight)
        {
            position.x = hitInfo.hitRect.getGlobalBounds().left - (boundingBoxPlayer.getGlobalBounds().width / 2 + epsilon) - epsilon;
        }
        else
        {
            position.x = hitInfo.hitRect.getGlobalBounds().left + hitInfo.hitRect.getGlobalBounds().width + (boundingBoxPlayer.getGlobalBounds().width / 2 + epsilon) - epsilon;
        }

        collisionSide = true;
    }
}

void Player::HandleObstacleCollision()
{
    isBlinking = true;
    blinkingTimer.restart();
    loseLifeCooldown.restart();
    DecreaseHealth();
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

void Player::AddBlinking()
{
    if (isBlinking)
    {
        if (IsPlayerProtected())
        {
            if (blinkingTimer.getElapsedTime().asSeconds() >= blinkingInterval)
            {

                if (isVisible)
                {
                    sprite.setColor(sf::Color(255, 255, 255, 0));
                    isVisible = !isVisible;
                }
                else
                {
                    isVisible = !isVisible;
                    sprite.setColor(sf::Color(255, 255, 255, 255));
                }

                blinkingTimer.restart();
            }
        }
    }
    else
    {
        isVisible = true;
        sprite.setColor(sf::Color(255, 255, 255, 255));
    }
}

void Player::CheckCollisionGround(const std::vector<Tile> &tiles)
{
    collisionGround = false;
    collisionTop = false;

    for (auto &tile : tiles)
    {
        sf::RectangleShape boundBox = tile.GetShape();
        Tile::Tile_Type type = tile.GetType();

        sf::FloatRect playerBounds = boundingBoxPlayer.getGlobalBounds();
        sf::FloatRect tileBounds = boundBox.getGlobalBounds();

        if (Math::CheckRectCollision(playerBounds, tileBounds))
        {
            if (!IsPlayerProtected() && type == Tile::Obstacle)
            {
                HandleObstacleCollision();
            }

            boundBox.setOutlineColor(sf::Color::Green);

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
    rebornAnimationTimer += dt;

    if (isRespawnTimerRestarted && respawnTimer.getElapsedTime().asSeconds() <= rebirth_animation_duration)
    {
        if (rebornAnimationTimer >= rebirth_animation_interval)
        {
            currentAnim++;
            rebornAnimationTimer = 0.0f;
        }

        if (currentAnim >= max_frames)
        {
            currentAnim = 0;
            isRespawnTimerRestarted = false;
        }

        sprite.setTextureRect(sf::IntRect(currentAnim * AnimationPlayer::TILE_SIZE, AnimationPlayer::REBORN_Y * AnimationPlayer::TILE_SIZE, AnimationPlayer::TILE_SIZE, AnimationPlayer::TILE_SIZE));
    }
    else
    {
        if (animationTimer >= animationInterval)
        {
            currentAnim++;

            // if the player stopped moving, reset its animation
            bool stopped = !moveLeft && !moveRight;
            if ((currentAnim >= max_frames || stopped) && !isJumping)
            {
                currentAnim = 0;
                ResetAnimation(AnimationPlayer::STOP_MOVING);
            }
            // if the player is jumping, play only one animation unit
            else if (currentAnim >= max_frames && isJumping)
            {
                currentAnim = 0;
                ResetAnimation(AnimationPlayer::STOPP_JUMPING);
            }
            animationTimer = 0.f;
        }
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

        respawnTimer.restart();
        isRespawnTimerRestarted = true;
        position = respawnPos;
        velocity.y = 0;
        sprite.setScale(scale, scale);
        atRespawnPos = true;
    }
}

void Player::HandleRespawn(bool respawn)
{
    if (respawn)
    {
        Respawn();
    }
}

void Player::UpdateView(bool moveRight, bool moveLeft)
{
    if (moveRight)
    {
        sprite.setScale(scale, scale);
        sprite.setTextureRect(sf::IntRect(currentAnim * AnimationPlayer::TILE_SIZE, AnimationPlayer::MOVING_Y * AnimationPlayer::TILE_SIZE, AnimationPlayer::TILE_SIZE, AnimationPlayer::TILE_SIZE));
    }
    else if (moveLeft)
    {
        sprite.setScale(-scale, scale);
        sprite.setTextureRect(sf::IntRect(currentAnim * AnimationPlayer::TILE_SIZE, AnimationPlayer::MOVING_Y * AnimationPlayer::TILE_SIZE, AnimationPlayer::TILE_SIZE, AnimationPlayer::TILE_SIZE));
    }

    sprite.setPosition(position);
    boundingBoxPlayer.setPosition(sprite.getPosition().x - 16, sprite.getPosition().y + 10);
}

void Player::Draw(const std::shared_ptr<sf::RenderTarget> rt)
{
    rt->draw(sprite);
    rt->draw(boundingBoxPlayer);
    DrawRay(rt, ray.start, ray.end);
}

void Player::DrawRay(const std::shared_ptr<sf::RenderTarget> &rt, const sf::Vector2f start, const sf::Vector2f end, sf::Color color)
{
    sf::VertexArray ray(sf::Lines, 2);

    ray[0].position = start;
    ray[0].color = color;
    ray[1].position = end;
    ray[1].color = color;

    rt->draw(ray);
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
        isBlinking = false;
        return false;
    }

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