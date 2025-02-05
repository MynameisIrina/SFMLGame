#include "Player.h"
#include <iostream>
#include "Utilities.h"
#include "ProjectilePool.h"

Player::Player(const std::shared_ptr<TextureLoader> &txLoader, ProjectilePool &projectilePool) : txLoader(txLoader), projectilePool(projectilePool) {}

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
    projectilesCount = projectilesAmount;
    maxProjectileCount = projectilesAmount;

    sprite = txLoader->SetSprite(TextureLoader::TextureType::Player);
    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
    sprite.setScale(scale, scale);

    boundingBoxPlayer = Utilities::CreateBoundingBox(sprite, position);
    boundingBoxPlayer.setSize(sf::Vector2f(sprite.getGlobalBounds().width - 2 * boundingBoxOffsetX, sprite.getGlobalBounds().height));
}

void Player::Update(const bool moveRight, const bool moveLeft, const bool shoot, const float leftBound, const bool respawn, const float dt, const std::vector<Tile> &tiles)
{
    // std::bitset<4> stateBits(state);
    // std::cout << "Player States: " << stateBits << std::endl;

    InputToState(moveRight, moveLeft, shoot, respawn);

    if (IfStateActive(State::Moving))
    {
        HandleHorizontalMovement(dt, leftBound);
    }

    if (IfStateActive(State::Respawning))
    {
        HandleRespawn();
    }

    if (IfStateActive(State::Shooting))
    {
        HandleShooting(shoot, dt);
    }
    else
    {
        ClearState(State::Shooting);
        // prevent continuous shooting
        isShooting = false;
    }

    HandleVerticalMovement(dt);
    CalculateCurrentAnimation(dt);
    CheckCollisionGround(tiles);
    CheckCollisionSide(tiles);
    HandleFalling();
    HandleBlinking();
    projectilePool.Update(dt);
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

    if (respawn)
    {
        SetState(State::Respawning);
    }
    else
    {
        // clear state in the next frame
        ClearState(State::Respawning);
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

void Player::CheckCollisionSide(const std::vector<Tile> &tiles)
{
    collisionSide = false;

    int direction = CalculateDirection();

    sf::Vector2f rayMiddle_Start = sprite.getPosition();
    sf::Vector2f rayMiddle_End = rayMiddle_Start + sf::Vector2f(direction * boundingBoxPlayer.getSize().x + epsilon, 0);

    ray = RayCast::Ray(rayMiddle_Start, rayMiddle_End);

    auto hit = RayCast::DoRaycast(rayMiddle_Start, rayMiddle_End, tiles);
    //
    if (hit)
    {

        RayCast::RayCastHit hitInfo = hit.value();

        if (!IsPlayerProtected() && hitInfo.typeTile == Tile::Obstacle)
        {
            HandleObstacleCollision();
        }

        if (!IsPlayerProtected() && hitInfo.typeTile == Tile::Enemy)
        {
            HandleEnemyCollision();
        }

        if (direction > 0)
        {
            position.x = hitInfo.hitRect.getGlobalBounds().left - (boundingBoxPlayer.getGlobalBounds().width) + epsilon;
        }
        else
        {
            position.x = hitInfo.hitRect.getGlobalBounds().left + hitInfo.hitRect.getGlobalBounds().width + boundingBoxPlayer.getGlobalBounds().width - (2 * epsilon);
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

void Player::Jump(const bool jump, const float dt)
{
    if (jump && !IfStateActive(State::Jumping))
    {
        SetState(State::Jumping);
        collisionGround = false;
        velocity.y = -jumpVelocity;
    }
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
            sprite.setColor(isVisible ? Utilities::GetTransparentColor(sf::Color(255,255,255)) : Utilities::GetOpaqueColor(sf::Color(255,255,255)));
            isVisible = !isVisible;
            blinkingTimer.restart();
        }
    }
    else
    {
        isVisible = true;
        sprite.setColor(Utilities::GetOpaqueColor(sf::Color(255,255,255)));
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

            if (!IsPlayerProtected() && type == Tile::Enemy)
            {
                HandleEnemyCollision();
            }

            boundBox.setOutlineColor(sf::Color::Green);

            float overlapTop = playerBounds.top - (tileBounds.top + tileBounds.height);
            float overlapBottom = tileBounds.top - (playerBounds.top + playerBounds.height);

            if (overlapBottom <= 0.f)
            {
                // snapping
                position.y = tileBounds.top - playerBounds.getSize().y / 2;

                collisionGround = true;
                ClearState(State::Jumping);
                continue;
            }
            if (overlapTop <= 0.f)
            {
                // snapping
                position.y = (tileBounds.top + tileBounds.height) + playerBounds.getSize().y / 2;

                collisionTop = true;
                continue;
            }
        }
    }
}

void Player::CalculateCurrentAnimation(const float dt)
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

        if (currentAnim >= maxFrames)
        {
            currentAnim = 0;
            isRespawnTimerRestarted = false;
        }

        sprite.setTextureRect(sf::IntRect(currentAnim * tileSize + TextureLoader::playerOffsetX, AnimationPlayer::REBORN_Y * TextureLoader::rectHeightPlayer, TextureLoader::rectWidthPlayer, TextureLoader::rectHeightPlayer));
    }
    else
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

        ClearState(State::Blinking);
    }
}

void Player::UpdateView(bool moveRight, bool moveLeft)
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
    boundingBoxPlayer.setPosition(sprite.getPosition().x + boundingBoxOffsetX, sprite.getPosition().y);
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

sf::Vector2f Player::GetPosition() const
{
    return position;
}