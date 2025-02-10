#include "Player.h"
#include <iostream>
#include "Utilities.h"
#include "ProjectilePool.h"
#include "Enemy.h"
#include "RespawnManager.h"

Player::Player(const std::shared_ptr<TextureLoader> &txLoader, ProjectilePool &projectilePool, std::shared_ptr<AudioManager> &audioManager) : txLoader(txLoader), projectilePool(projectilePool), audioManager(audioManager) {}

void Player::Initialize(const sf::Vector2f position, const int maxHealth, const int projectilesAmount, const float scale, const float positionThresholdY)
{
    this->position = position;
    this->maxHealth = maxHealth;
    this->scale = scale;
    this->positionThresholdY = positionThresholdY;

    maxPosition = position;
    saveLastPos = position;
    respawnPos = position;
    health = maxHealth;
    condition = Normal;
    state = 0;
    projectilesCount = projectilesAmount;
    maxProjectileCount = projectilesAmount;
    previousPosition = position;

    sprite = txLoader->SetSprite(TextureLoader::TextureType::Player);
    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
    sprite.setScale(scale, scale);

    boundingBoxPlayer = Utilities::CreateBoundingBox(sprite, position);
    boundingBoxPlayer.setSize(sf::Vector2f(sprite.getGlobalBounds().width - 2 * boundingBoxOffsetX, sprite.getGlobalBounds().height));
}

void Player::Update(const std::shared_ptr<RespawnManager> &respawnManager, const std::shared_ptr<Camera> &camera, const bool moveRight, const bool moveLeft, const bool shoot, const float leftBound, const bool respawn, const bool exchangeCoins, const float dt, const std::vector<sf::RectangleShape> &tilesShapes, std::vector<sf::RectangleShape> &enemiesShapes, std::vector<sf::RectangleShape> &obstaclesShapes)
{
    InputToState(moveRight, moveLeft, shoot, respawn);

    if (IfStateActive(State::Moving) && !IsInRebirth())
    {
        HandleHorizontalMovement(dt, leftBound);
    }

    if (IfStateActive(State::Respawning))
    {
        respawnManager->RespawnAllEntities();
    }

    if (IfStateActive(State::Shooting) && !IsInRebirth())
    {
        HandleShooting(shoot, dt);
    }
    else
    {
        ClearState(State::Shooting);

        // prevent continuous shooting
        isShooting = false;
    }

    if (!IsInRebirth())
    {
        HandleVerticalMovement(dt);
        CheckCollisionGround(tilesShapes, enemiesShapes, obstaclesShapes);
        CheckCollisionSide(tilesShapes, enemiesShapes, obstaclesShapes);
        HandleFalling();
        HandleBlinking();
        projectilePool.Update(camera, dt);
        HandleProjectileReset();
        HandleCoinLifeExchange(exchangeCoins);
    }

    CalculateCurrentAnimation(dt);
    UpdateView(moveRight, moveLeft);
}

void Player::InputToState(bool moveRight, bool moveLeft, bool shoot, bool respawn)
{
    if (moveRight || moveLeft)
    {
        SetState(State::Moving);
    }
    else
    {
        ClearState(State::Moving);
    }

    if (shoot)
    {
        SetState(State::Shooting);
    }
    else
    {
        ClearState(State::Shooting);
    }
}

void Player::HandleFalling()
{
    if (!IsPlayerProtected() && position.y > positionThresholdY)
    {
        DecreaseHealth();
        SetState(State::Respawning);
        loseLifeCooldown.restart();
    }
}

void Player::HandleHorizontalMovement(float dt, float leftBound)
{

    const int direction = CalculateDirection();

    if (!collisionSide)
    {
        if (direction > 0)
        {
            position.x += velocity.x * dt;
        }
        else
        {
            if (position.x > leftBound + 27.f)
            {
                position.x -= velocity.x * dt;
            }
        }
    }
    else
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
    if (!collisionGround)
    {
        velocity.y += gravity * dt;
        collisionTop = false;
    }
    else
    {
        velocity.y = 0;
    }

    if (collisionTop)
    {
        velocity.y = 0;
    }

    position.y += velocity.y * dt;
}

void Player::CheckCollisionSide(const std::vector<sf::RectangleShape> &tilesShapes, std::vector<sf::RectangleShape> &enemiesShapes, std::vector<sf::RectangleShape> &obstaclesShapes)
{

    collisionSide = false;

    int direction = CalculateDirection();

    sf::Vector2f rayMiddle_Start = sprite.getPosition();
    sf::Vector2f rayMiddle_End = rayMiddle_Start + sf::Vector2f(direction * boundingBoxPlayer.getSize().x + epsilon, 0);

    ray = RayCast::Ray(rayMiddle_Start, rayMiddle_End);

    auto hitTiles = RayCast::DoRaycast(rayMiddle_Start, rayMiddle_End, tilesShapes);
    auto hitEnemies = RayCast::DoRaycast(rayMiddle_Start, rayMiddle_End, enemiesShapes);
    auto hitObstacles = RayCast::DoRaycast(rayMiddle_Start, rayMiddle_End, obstaclesShapes);

    RayCast::RayCastHit hitInfo;
    bool hasCollision = false;

    if (hitTiles)
    {
        hitInfo = hitTiles.value();
        hasCollision = true;
    }
    else if (hitEnemies)
    {
        hitInfo = hitEnemies.value();
        hasCollision = true;
        if (!IsPlayerProtected())
        {
            HandleEnemyCollision();
        }
    }
    else if (hitObstacles)
    {
        hitInfo = hitObstacles.value();
        hasCollision = true;

        if (!IsPlayerProtected())
        {
            HandleObstacleCollision();
        }
    }

    if (hasCollision)
    {
        if (direction > 0)
        {
            position.x = hitInfo.hitRect.getGlobalBounds().left - (sprite.getGlobalBounds().getSize().x / 2.f) - 0.5f;
        }
        else
        {
            position.x = (hitInfo.hitRect.getGlobalBounds().left + hitInfo.hitRect.getGlobalBounds().width) + (sprite.getGlobalBounds().getSize().x / 2.f) - 2 * epsilon + 0.5f;
        }

        collisionSide = true;
    }
}

void Player::HandleObstacleCollision()
{
    SetState(State::Blinking);
    blinkingTimer.restart();
    loseLifeCooldown.restart();
    DecreaseHealth();
}

void Player::HandleEnemyCollision()
{
    SetState(State::Blinking);
    blinkingTimer.restart();
    loseLifeCooldown.restart();
    DecreaseHealth();
}

void Player::HandleShooting(const bool shoot, const float dt)
{
    if (!isShooting && projectilesCount > 0)
    {
        Projectile *projectile = projectilePool.GetProjectile();

        if (projectile != nullptr)
        {
            const int direction = CalculateDirection();
            projectile->position = direction > 0 ? sf::Vector2f(position.x + projectileOffsetX, position.y) : sf::Vector2f(position.x - projectileOffsetX, position.y);
            projectile->velocity = direction > 0 ? projectileVelocity : -projectileVelocity;
            isShooting = true;
            projectilesCount--;

            if (projectilesCount == 0)
            {
                projectileResetTimer.restart();
            }
        }
    }

    // prevent continuous shooting
    if (!IfStateActive(State::Shooting) && isShooting)
    {
        isShooting = false;
    }
}

int Player::CalculateDirection()
{
    bool isMovingRight = sprite.getScale().x > 0;
    int direction = isMovingRight ? 1.0f : -1.0f;

    return direction;
}

void Player::ResetAnimation(int animYIndex)
{
    sprite.setTextureRect(sf::IntRect(TextureLoader::playerX + TextureLoader::playerOffsetX, TextureLoader::playerY * TextureLoader::rectHeightPlayer, TextureLoader::rectWidthPlayer, TextureLoader::rectHeightPlayer));
}

void Player::HandleBlinking()
{
    if (IfStateActive(State::Blinking) && IsPlayerProtected())
    {
        if (blinkingTimer.getElapsedTime().asSeconds() >= blinkingInterval)
        {
            sprite.setColor(isVisible ? Utilities::GetTransparentColor(normalColor) : Utilities::GetOpaqueColor(normalColor));
            isVisible = !isVisible;
            blinkingTimer.restart();
        }
    }
    else
    {
        isVisible = true;
        sprite.setColor(Utilities::GetOpaqueColor(normalColor));
    }
}

void Player::HandleRebirthAnimation()
{
    if (rebornAnimationTimer >= rebirth_animation_interval)
    {
        currentAnim++;
        rebornAnimationTimer = 0.0f;
    }

    if (currentAnim >= maxFrames)
    {
        currentAnim = 0;
        isRespawnTimerRestarted = false;
    }

}

void Player::HandleMovementAnimation()
{
    if (animationTimer >= animationInterval)
        {
            currentAnim += 1;

            // if the player stopped moving, reset its animation
            bool stopped = !IfStateActive(State::Moving);
            if ((currentAnim >= maxFrames || stopped) && !IfStateActive(State::Jumping))
            {
                currentAnim = 0;
                ResetAnimation(AnimationPlayer::STOP_MOVING);
            }
            // if the player is jumping, play only one animation unit
            else if (currentAnim >= maxFrames && IfStateActive(State::Jumping))
            {
                currentAnim = 0;
                ResetAnimation(AnimationPlayer::STOPP_JUMPING);
            }
            animationTimer = 0.f;
        }
}

void Player::CheckCollisionGround(const std::vector<sf::RectangleShape> &tiles, std::vector<sf::RectangleShape> &enemies, std::vector<sf::RectangleShape> &obstaclesShapes)
{
    collisionGround = false;
    collisionTop = false;

    sf::FloatRect playerBounds = boundingBoxPlayer.getGlobalBounds();
    const float playerHeight = playerBounds.getSize().y;
    const float playerHalfHeight = playerHeight * 0.5f;

    for (auto &boundBox : tiles)
    {
        sf::FloatRect tileBounds = boundBox.getGlobalBounds();

        if (Math::CheckRectCollision(playerBounds, tileBounds))
        {
            float overlapTop = playerBounds.top - (tileBounds.top + tileBounds.height);
            float overlapBottom = tileBounds.top - (playerBounds.top + playerBounds.height);

            if (overlapBottom <= 0.f)
            {
                // snapping
                position.y = tileBounds.top - playerHalfHeight;

                collisionGround = true;
                ClearState(State::Jumping);
                break;
            }
            if (overlapTop <= 0.f)
            {
                // snapping
                position.y = (tileBounds.top + tileBounds.height) + playerHalfHeight;

                collisionTop = true;
                break;
            }
        }
    }

    for (auto &boundBox : enemies)
    {
        sf::FloatRect tileBounds = boundBox.getGlobalBounds();

        if (Math::CheckRectCollision(playerBounds, tileBounds))
        {
            float overlapBottom = tileBounds.top - (playerBounds.top + playerBounds.height);

            if (overlapBottom <= 0.f)
            {
                // snapping
                position.y = tileBounds.top - playerHalfHeight;

                if (!IsPlayerProtected())
                {
                    HandleEnemyCollision();
                }
                collisionGround = true;
                ClearState(State::Jumping);
                break;
            }
        }
    }

    for (auto &boundBox : obstaclesShapes)
    {
        sf::FloatRect tileBounds = boundBox.getGlobalBounds();

        if (Math::CheckRectCollision(playerBounds, tileBounds))
        {

            float overlapBottom = tileBounds.top - (playerBounds.top + playerBounds.height);

            if (overlapBottom <= 0.f)
            {
                // snapping
                position.y = tileBounds.top - playerHalfHeight;

                if (!IsPlayerProtected())
                {
                    HandleObstacleCollision();
                }

                collisionGround = true;
                ClearState(State::Jumping);
                break;
            }
        }
    }
}

void Player::Jump(const bool jump, const float dt)
{
    if (jump && !IfStateActive(State::Jumping))
    {
        SetState(State::Jumping);
        // audioManager->PlaySound("jump");
        collisionGround = false;
        velocity.y = -jumpVelocity;
    }
}

void Player::CalculateCurrentAnimation(const float dt)
{
    animationTimer += dt;
    rebornAnimationTimer += dt;

    if (IsInRebirth())
    {
        HandleRebirthAnimation();
    }
    else
    {
        HandleMovementAnimation();
    }
}

void Player::HandleRespawn()
{

    if (position != respawnPos)
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

        ResetHealth();
        ResetCoins();
        ResetProjectiles();
        ResetBlinking();
        projectilePool.Clear();

        state = 0;
    }
}

void Player::UpdateView(bool moveRight, bool moveLeft)
{
    if (!IsInRebirth())
    {
        if (moveRight)
        {
            sprite.setScale(scale, scale);
            sprite.setTextureRect(sf::IntRect(currentAnim * tileSize + TextureLoader::playerOffsetX, TextureLoader::playerY * TextureLoader::rectHeightPlayer, TextureLoader::rectWidthPlayer, TextureLoader::rectHeightPlayer));
        }
        else if (moveLeft)
        {
            sprite.setScale(-scale, scale);
            sprite.setTextureRect(sf::IntRect(currentAnim * tileSize + TextureLoader::playerOffsetX, TextureLoader::playerY * TextureLoader::rectHeightPlayer, TextureLoader::rectWidthPlayer, TextureLoader::rectHeightPlayer));
        }

        sprite.setPosition(position);
        boundingBoxPlayer.setPosition(position.x + boundingBoxOffsetX, position.y);
    }
    else
    {
        sprite.setTextureRect(sf::IntRect(currentAnim * tileSize + TextureLoader::playerOffsetX, (AnimationPlayer::REBORN_Y * TextureLoader::rectHeightPlayer) - (31 - TextureLoader::rectHeightPlayer), TextureLoader::rectWidthPlayer, 31));
        sprite.setPosition(position - sf::Vector2f(0, 4 * rebirthVerticaloffset));
        boundingBoxPlayer.setPosition(position.x + boundingBoxOffsetX, position.y + rebirthVerticaloffset);
    }
}

void Player::Draw(const std::shared_ptr<sf::RenderTarget> rt) const
{
    rt->draw(sprite);
    rt->draw(boundingBoxPlayer);
    DrawRay(rt, ray.start, ray.end);
    projectilePool.Draw(rt);
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

void Player::SetState(State stateToSet)
{
    // bitwise OR operator
    state |= stateToSet;
}

void Player::ClearState(State stateToClear)
{
    // example: state to clear = 0001, current state = 0111
    // ~ negates state to clear (~0001 returns 1110)
    // & returns state without state that needs to be cleareed (0111 & 1110 = 0110)

    state &= ~stateToClear;
}

bool Player::IfStateActive(State stateToCheck)
{
    return (state & stateToCheck) != 0;
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

        if (health == 0)
        {
            SetState(State::Respawning);
        }
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
        ClearState(State::Blinking);
        return false;
    }

    return true;
}

sf::Vector2f Player::GetMaxPosition() const
{
    return maxPosition;
}

int Player::GetProjectilesCount()
{
    return projectilesCount;
}

std::vector<Projectile *> Player::GetActiveProjectiles() const
{
    return projectilePool.GetActiveProjectiles();
}

sf::RectangleShape Player::GetBoundingBox() const
{
    return boundingBoxPlayer;
}

void Player::ResetProjectilesCount()
{
    projectilesCount = maxProjectileCount;
}

void Player::PickUpCoin()
{
    coinsCollected++;
    audioManager->PlaySound("coinCollected");
}

int Player::GetCoins()
{
    return coinsCollected;
}

bool Player::IsInRebirth()
{
    return isRespawnTimerRestarted && respawnTimer.getElapsedTime().asSeconds() <= rebirth_animation_duration;
}

void Player::ResetHealth()
{
    health = maxHealth;
}

void Player::ResetCoins()
{
    coinsCollected = 0;
}

void Player::ResetProjectiles()
{
    projectilesCount = maxProjectileCount;
}

void Player::ResetBlinking()
{
    isVisible = true;
    sprite.setColor(Utilities::GetOpaqueColor(sf::Color(255, 255, 255)));
}

void Player::HandleProjectileReset()
{
    if (projectilesCount > 0)
        return;

    if (projectilesCount <= 0)
    {
        if (projectileResetTimer.getElapsedTime().asSeconds() >= projectileResetInterval)
        {
            ResetProjectiles();
        }
    }
}

void Player::DecreaseCoins()
{
    coinsCollected--;
}

void Player::HandleCoinLifeExchange(bool exchangeCoins)
{
    if (exchangeCoins && !coinsExchanged)
    {
        if (coinsCollected > 0 && health < maxHealth)
        {
            DecreaseCoins();
            IncreaseHealth();

            coinsExchanged = true;
        }
    }

    if (!exchangeCoins && coinsExchanged)
    {
        coinsExchanged = false;
    }
}

sf::Vector2f Player::GetPosition() const
{
    return position;
}