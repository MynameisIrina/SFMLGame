#include "player/Player.h"
#include "utilities/Math.h"
#include "graphics/TextureLoader.h"
#include "ui/HealthBar.h"
#include "level/Tile.h"
#include "player/projectiles/ProjectilePool.h"
#include "managers/AudioManager.h"
#include "camera/Camera.h"
#include "utilities/Utilities.h"
#include "managers/RespawnManager.h"

Player::Player(const std::shared_ptr<TextureLoader> &txLoader, std::unique_ptr<ProjectilePool> projectilePool, std::shared_ptr<AudioManager> &audioManager) : txLoader(txLoader), projectilePool(std::move(projectilePool)), audioManager(audioManager) {}

void Player::Initialize(const sf::Vector2f &position, const int maxHealth, const int projectilesAmount, const float scale, const float positionThresholdY)
{
    this->position = position;
    this->maxHealth = maxHealth;
    this->scale = scale;
    this->positionThresholdY = positionThresholdY;

    maxPosition = position;
    saveLastPos = position;
    respawnPos = position;
    health = maxHealth;
    condition = PlayerCondition::Normal;
    state = 0;
    projectilesCount = projectilesAmount;
    maxProjectileCount = projectilesAmount;
    previousPosition = position;

    sprite = txLoader->SetSprite(TextureLoader::TextureType::Player);
    sprite.setOrigin(sprite.getLocalBounds().width * 0.5f, sprite.getLocalBounds().height * 0.5f);
    sprite.setScale(scale, scale);

    boundingBoxPlayer = Utilities::CreateBoundingBox(sprite, position);
    boundingBoxPlayer.setSize(sf::Vector2f(sprite.getGlobalBounds().width - boundingBoxResizeX, sprite.getGlobalBounds().height - boundingBoxResizeY));
    boundingBoxPlayer.setOrigin(boundingBoxPlayer.getSize().x * 0.5f, 0);
}

void Player::Update(const std::shared_ptr<RespawnManager> &respawnManager, const std::shared_ptr<Camera> &camera, const bool moveRight, const bool moveLeft, const bool shoot, const bool respawn,
                    const bool exchangeCoins, const float dt, const std::vector<sf::RectangleShape> &tilesShapes, std::vector<sf::RectangleShape> &enemiesShapes, std::vector<sf::RectangleShape> &flyingEnemiesShapes,
                    std::vector<sf::RectangleShape> &obstaclesShapes)
{

    UpdateAllTimers(dt);

    if (IsInRebirth())
    {
        UpdateRebirthView();
        HandleRebirthAnimation();
        return;
    }

    InputToState(moveRight, moveLeft, shoot, respawn);

    if (IfStateActive(State::Respawning))
    {
        respawnManager->RespawnAllEntities();
    }

    if (IfStateActive(State::Moving))
    {
        HandleHorizontalMovement(camera, dt);
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

    HandleMovementAnimation();
    HandleVerticalMovement(dt);
    UpdateView(moveRight, moveLeft);
    CheckCollisionGround(tilesShapes, enemiesShapes, flyingEnemiesShapes, obstaclesShapes);
    HandleBlinking();
    projectilePool->Update(camera, dt);
    HandleProjectileReset(dt);
    HandleCoinLifeExchange(exchangeCoins);
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
    DecreaseHealth();
    SetState(State::Respawning);
    loseLifeCooldown.restart();
}

void Player::HandleHorizontalMovement(const std::shared_ptr<Camera> &camera, float dt)
{
    if (collisionSide)
    {
        velocity.x = 0;
        return;
    }

    const Direction direction = CalculateDirection();
    const float movementDelta = velocity.x * dt;
    const float leftCameraBound = camera->CalculateLeftBound();
    velocity.x = horizontalVelocity;

    if (direction == Direction::Right)
    {
        position.x += movementDelta;
    }
    else if (direction == Direction::Left)
    {
        if (position.x > leftCameraBound + leftBoundaryOffset)
        {
            position.x -= movementDelta;
        }
    }
}

void Player::HandleVerticalMovement(float dt)
{
    if (!collisionGround)
    {
        velocity.y += gravity * dt;
        collisionTop = false;
    }

    if (collisionTop || collisionGround)
    {
        velocity.y = 0;
    }

    position.y += velocity.y * dt;

    if (IsFalling())
    {
        HandleFalling();
    }
}

void Player::HandleBlinking()
{
    if (!IfStateActive(State::Blinking))
    {
        ResetBlinking();
        return;
    }

    if (blinkingTimer.getElapsedTime().asSeconds() >= blinkingInterval)
    {
        sprite.setColor(isVisible ? Utilities::GetTransparentColor(normalColor) : Utilities::GetOpaqueColor(normalColor));
        isVisible = !isVisible;
        blinkingTimer.restart();
    }
}

void Player::HandleObstacleCollision()
{
    if (IsPlayerProtected())
        return;

    DecreaseHealth();
    StartBlinking();
}

void Player::HandleEnemyCollision()
{
    if (IsPlayerProtected())
        return;

    DecreaseHealth();
    StartBlinking();
}

void Player::HandleFlyingEnemyCollision()
{
    if (IsPlayerProtected())
        return;

    DecreaseHealth();
    StartBlinking();
}

void Player::HandleShooting(const bool shoot, const float dt)
{
    if (!isShooting && projectilesCount > 0)
    {
        Projectile *projectile = projectilePool->GetProjectile();

        if (projectile != nullptr)
        {
            const Direction direction = CalculateDirection();
            const sf::Vector2f boundingBoxPosition = boundingBoxPlayer.getPosition();
            const sf::Vector2f boundingBoxHalfSize = boundingBoxPlayer.getSize() * 0.5f;

            projectile->position = (direction == Direction::Right) ? sf::Vector2f(boundingBoxPosition.x + boundingBoxHalfSize.x, boundingBoxPosition.y + boundingBoxHalfSize.y) : sf::Vector2f(boundingBoxPosition.x - boundingBoxHalfSize.x, boundingBoxPosition.y + boundingBoxHalfSize.y);
            projectile->velocity = (direction == Direction::Right) ? projectileVelocity : -projectileVelocity;
            isShooting = true;
            projectilesCount--;

            projectileResetTimer.restart();
        }
    }

    // prevent continuous shooting
    if (!IfStateActive(State::Shooting) && isShooting)
    {
        isShooting = false;
    }
}

Player::Direction Player::CalculateDirection()
{
    bool isMovingRight = sprite.getScale().x > 0;
    return isMovingRight ? Direction::Right : Direction::Left;
}

bool Player::IsFalling()
{
    return position.y > positionThresholdY;
}

void Player::ResetAnimation()
{
    sprite.setTextureRect(sf::IntRect(TextureLoader::playerX, TextureLoader::playerY * TextureLoader::rectHeightPlayer, TextureLoader::rectWidthPlayer, TextureLoader::rectHeightPlayer));
}

void Player::HandleRebirthAnimation()
{
    if (rebirthAnimationTimer >= rebirthAnimationInterval)
    {
        currentRebirthAnim++;
        rebirthAnimationTimer = 0.0f;
    }

    if (currentRebirthAnim >= maxFrames)
    {
        currentRebirthAnim = 0;
        isRespawnTimerRestarted = false;
    }
}

void Player::HandleMovementAnimation()
{
    if (movementAnimationTimer >= animationInterval)
    {
        currentAnim++;

        // if the player stopped moving, reset its animation
        bool stopped = !IfStateActive(State::Moving);
        if ((currentAnim >= maxFrames || stopped))
        {
            currentAnim = 0;
            ResetAnimation();
        }

        movementAnimationTimer = 0.f;
    }
}

void Player::HandleGroundCollision(const sf::FloatRect &otherBounds, const float playerHalfHeight)
{
    const float offset = 1.f;
    position.y = otherBounds.top - playerHalfHeight - boundingBoxOffsetY + offset;
    collisionGround = true;
    ClearState(Player::State::Jumping);
}

void Player::HandleTopCollision(const sf::FloatRect &otherBounds, const float playerHalfHeight)
{
    const float offset = 1.f;
    position.y = (otherBounds.top + otherBounds.height) + playerHalfHeight - offset;
    collisionTop = true;
}

void Player::HandleLeftCollision(const sf::FloatRect &otherBounds, const float playerHalfWidth)
{
    Direction dir = CalculateDirection();
    float offset = (dir == Direction::Right) ? -boundingBoxOffsetX : boundingBoxOffsetX;

    position.x = (otherBounds.left + otherBounds.width) + playerHalfWidth + offset;
    collisionSide = true;
}

void Player::HandleRightCollision(const sf::FloatRect &otherBounds, const float playerHalfWidth)
{
    Direction dir = CalculateDirection();
    float offset = (dir == Direction::Right) ? -boundingBoxOffsetX : boundingBoxOffsetX;

    position.x = otherBounds.left - playerHalfWidth + offset;
    collisionSide = true;
}

Player::CollisionInfo Player::CalculateCollision(const sf::FloatRect &playerBounds, const sf::FloatRect &otherBounds)
{
    if (!Math::CheckRectCollision(playerBounds, otherBounds))
        return Player::CollisionInfo();

    const float overlapTop = playerBounds.top - (otherBounds.top + otherBounds.height);
    const float overlapBottom = otherBounds.top - (playerBounds.top + playerBounds.height);
    const float overlapLeftSide = playerBounds.left - (otherBounds.left + otherBounds.width);
    const float overlapRightSide = otherBounds.left - (playerBounds.left + playerBounds.width);

    return Player::CollisionInfo(overlapBottom, overlapTop, overlapLeftSide, overlapRightSide, true);
}

bool Player::CheckPlatformsCollision(const sf::FloatRect &playerBounds, const std::vector<sf::RectangleShape> &tiles, const float playerHalfHeight, const float playerHalfWidth)
{
    for (auto &boundBox : tiles)
    {
        sf::FloatRect tileBounds = boundBox.getGlobalBounds();
        CollisionInfo collisionInfo = CalculateCollision(playerBounds, tileBounds);

        if (!collisionInfo.hasCollision)
            continue;

        if (collisionInfo.overlapBottom <= 0.f && collisionInfo.overlapBottom >= maxOverlap)
        {
            HandleGroundCollision(tileBounds, playerHalfHeight);
            return true;
        }

        if (collisionInfo.overlapLeftSide <= 0.f && collisionInfo.overlapLeftSide >= maxOverlap)
        {

            HandleLeftCollision(tileBounds, playerHalfWidth);
            return true;
        }

        if (collisionInfo.overlapRightSide <= 0.f && collisionInfo.overlapRightSide >= maxOverlap)
        {
            HandleRightCollision(tileBounds, playerHalfWidth);
            return true;
        }

        if (collisionInfo.overlapTop <= 0.f && collisionInfo.overlapTop >= maxOverlap)
        {
            HandleTopCollision(tileBounds, playerHalfHeight);
            return true;
        }
    }

    return false;
}

bool Player::CheckEnemiesCollision(const sf::FloatRect &playerBounds, const std::vector<sf::RectangleShape> &enemies, const float playerHalfHeight, const float playerHalfWidth)
{
    for (auto &boundBox : enemies)
    {
        sf::FloatRect enemyBounds = boundBox.getGlobalBounds();
        CollisionInfo collisionInfo = CalculateCollision(playerBounds, enemyBounds);

        if (!collisionInfo.hasCollision)
            continue;

        if (collisionInfo.overlapBottom <= 0.f && collisionInfo.overlapBottom >= maxOverlap)
        {
            HandleGroundCollision(enemyBounds, playerHalfHeight);
            HandleEnemyCollision();
            return true;
        }

        if (collisionInfo.overlapTop <= 0.f && collisionInfo.overlapTop >= maxOverlap)
        {
            HandleTopCollision(enemyBounds, playerHalfHeight);
            HandleEnemyCollision();
            return true;
        }

        if (collisionInfo.overlapLeftSide <= 0.f && collisionInfo.overlapLeftSide >= maxOverlap)
        {
            HandleLeftCollision(enemyBounds, playerHalfWidth);
            HandleEnemyCollision();
            return true;
        }

        if (collisionInfo.overlapRightSide <= 0.f && collisionInfo.overlapRightSide >= maxOverlap)
        {
            HandleRightCollision(enemyBounds, playerHalfWidth);
            HandleEnemyCollision();
            return true;
        }
    }

    return false;
}

bool Player::CheckFlyingEnemiesCollision(const sf::FloatRect &playerBounds, std::vector<sf::RectangleShape> &flyingEnemiesShapes, const float playerHalfHeight, const float playerHalfWidth)
{
    if (IsPlayerProtected())
        return false;

    for (auto &boundBox : flyingEnemiesShapes)
    {
        sf::FloatRect enemyBounds = boundBox.getGlobalBounds();
        CollisionInfo collisionInfo = CalculateCollision(playerBounds, enemyBounds);

        if (!collisionInfo.hasCollision)
            continue;

        if (collisionInfo.overlapBottom <= 0.f && collisionInfo.overlapBottom >= maxOverlap)
        {
            HandleFlyingEnemyCollision();
            return true;
        }

        if (collisionInfo.overlapLeftSide <= 0.f && collisionInfo.overlapLeftSide >= maxOverlap)
        {
            HandleFlyingEnemyCollision();
            return true;
        }

        if (collisionInfo.overlapRightSide <= 0.f && collisionInfo.overlapRightSide >= maxOverlap)
        {
            HandleFlyingEnemyCollision();
            return true;
        }

        if (collisionInfo.overlapTop <= 0.f && collisionInfo.overlapTop >= maxOverlap)
        {
            HandleFlyingEnemyCollision();
            return true;
        }
    }

    return false;
}

bool Player::CheckObstaclesCollisions(const sf::FloatRect &playerBounds, const std::vector<sf::RectangleShape> &obstacles, const float playerHalfHeight, const float playerHalfWidth)
{
    for (const auto &obstacle : obstacles)
    {
        const sf::FloatRect &obstacleBounds = obstacle.getGlobalBounds();
        CollisionInfo collisionInfo = CalculateCollision(playerBounds, obstacleBounds);

        if (!collisionInfo.hasCollision)
            continue;

        if (collisionInfo.overlapBottom <= 0.f && collisionInfo.overlapBottom >= maxOverlap)
        {
            HandleGroundCollision(obstacleBounds, playerHalfHeight);
            if (!IsPlayerProtected())
            {
                HandleObstacleCollision();
            }
            return true;
        }
        if (collisionInfo.overlapLeftSide <= 0.f && collisionInfo.overlapLeftSide >= maxOverlap)
        {

            HandleLeftCollision(obstacleBounds, playerHalfWidth);
            HandleObstacleCollision();
            return true;
        }

        if (collisionInfo.overlapRightSide <= 0.f && collisionInfo.overlapRightSide >= maxOverlap)
        {

            HandleRightCollision(obstacleBounds, playerHalfWidth);
            HandleObstacleCollision();
            return true;
        }
    }
    return false;
}

void Player::CheckCollisionGround(const std::vector<sf::RectangleShape> &tiles, const std::vector<sf::RectangleShape> &enemies, std::vector<sf::RectangleShape> &flyingEnemies, const std::vector<sf::RectangleShape> &obstacles)
{
    collisionGround = false;
    collisionTop = false;
    collisionSide = false;

    const sf::FloatRect &playerBounds = boundingBoxPlayer.getGlobalBounds();
    const float playerHeight = playerBounds.getSize().y;
    const float playerHalfHeight = playerHeight;
    const float playerWidth = playerBounds.getSize().x;
    const float playerHalfWidth = playerBounds.getSize().x * 0.5f;

    if (CheckPlatformsCollision(playerBounds, tiles, playerHalfHeight, playerHalfWidth))
        return;
    if (CheckEnemiesCollision(playerBounds, enemies, playerHalfHeight, playerHalfWidth))
        return;
    if (CheckObstaclesCollisions(playerBounds, obstacles, playerHalfHeight, playerHalfWidth))
        return;
    if (CheckFlyingEnemiesCollision(playerBounds, flyingEnemies, playerHalfHeight, playerHalfWidth))
        return;
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

void Player::HandleRespawn()
{
    if (position == respawnPos)
        return;

    if (position.x > maxPosition.x)
    {
        maxPosition = position;
    }

    respawnTimer.restart();
    isRespawnTimerRestarted = true;
    position = respawnPos;

    velocity.y = 0;
    sprite.setScale(scale, scale);
    sprite.setPosition(respawnPos);

    ResetHealth();
    ResetCoins();
    ResetProjectilesCount();
    ResetBlinking();
    projectilePool->Clear();

    ClearState(State::Respawning);
}

void Player::UpdateAllTimers(const float dt)
{
    rebirthAnimationTimer += dt;
    movementAnimationTimer += dt;
}

void Player::UpdateView(bool moveRight, bool moveLeft)
{
    if (moveRight)
    {
        sprite.setScale(scale, scale);
        sprite.setTextureRect(sf::IntRect(currentAnim * AnimationCoordinates::tileSize, AnimationCoordinates::movingY * AnimationCoordinates::tileSize + TextureLoader::playerOffsetY, TextureLoader::rectWidthPlayer, TextureLoader::rectHeightPlayer - TextureLoader::playerOffsetY));
    }
    else if (moveLeft)
    {
        sprite.setScale(-scale, scale);
        sprite.setTextureRect(sf::IntRect(currentAnim * AnimationCoordinates::tileSize, AnimationCoordinates::movingY * AnimationCoordinates::tileSize + TextureLoader::playerOffsetY, TextureLoader::rectWidthPlayer, TextureLoader::rectHeightPlayer - TextureLoader::playerOffsetY));
    }
    else
    {
        sprite.setTextureRect(sf::IntRect(0, AnimationCoordinates::movingY * AnimationCoordinates::tileSize + TextureLoader::playerOffsetY, TextureLoader::rectWidthPlayer, TextureLoader::rectHeightPlayer - TextureLoader::playerOffsetY));
    }

    const Direction dir = CalculateDirection();
    const float offsetX = (dir == Direction::Right) ? boundingBoxOffsetX : -boundingBoxOffsetX;
    const float offsetY = boundingBoxOffsetY;

    sprite.setPosition(position);
    boundingBoxPlayer.setPosition(position.x + offsetX, position.y + offsetY);
}

void Player::UpdateRebirthView()
{
    sprite.setTextureRect(sf::IntRect(currentRebirthAnim * tileSize, AnimationCoordinates::rebornY * tileSize + TextureLoader::playerOffsetY, TextureLoader::rectWidthPlayer, TextureLoader::rectHeightPlayer  - TextureLoader::playerOffsetY));
    sprite.setPosition(position);
    boundingBoxPlayer.setPosition(position.x, position.y);
}

void Player::Draw(const std::shared_ptr<sf::RenderTarget> &rt) const
{
    rt->draw(sprite);
    projectilePool->Draw(rt);
}

void Player::SetState(State stateToSet)
{
    state |= stateToSet;
}

void Player::ClearState(State stateToClear)
{
    state &= ~stateToClear;
}

bool Player::IfStateActive(State stateToCheck)
{
    return (state & stateToCheck) != 0;
}

void Player::IncreaseHealth()
{
    if (health >= maxHealth)
        return;

    health++;
}

void Player::DecreaseHealth()
{
    if (health < 0)
        return;

    health--;

    if (health == 0)
    {
        SetState(State::Respawning);
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
    if (loseLifeCooldown.getElapsedTime().asSeconds() < loseLifeDelay)
        return true;

    ClearState(State::Blinking);
    return false;
}

sf::Vector2f Player::GetMaxPosition() const
{
    return maxPosition;
}

int Player::GetProjectilesCount() const
{
    return projectilesCount;
}

std::vector<Projectile *> Player::GetActiveProjectiles() const
{
    return projectilePool->GetActiveProjectiles();
}

sf::RectangleShape Player::GetBoundingBox() const
{
    return boundingBoxPlayer;
}

void Player::PickUpCoin()
{
    coinsCollected++;
    audioManager->PlaySound("coinCollected");
}

int Player::GetCoins() const
{
    return coinsCollected;
}

bool Player::IsInRebirth() const
{
    return isRespawnTimerRestarted && respawnTimer.getElapsedTime().asSeconds() <= rebirthAnimationDuration;
}

void Player::ResetHealth()
{
    health = maxHealth;
}

void Player::ResetCoins()
{
    coinsCollected = 0;
}

void Player::ResetProjectilesCount()
{
    projectilesCount = maxProjectileCount;
}

void Player::ResetBlinking()
{
    ClearState(State::Blinking);
    isVisible = true;
    sprite.setColor(Utilities::GetOpaqueColor(normalColor));
}

void Player::IncreaseProjectiles()
{
    if (projectilesCount >= maxProjectileCount)
        return;

    projectilesCount++;
}

void Player::HandleProjectileReset(const float dt)
{
    if (projectilesCount >= maxProjectileCount)
        return;

    projectileAccumulatedTime += dt;

    if (projectileAccumulatedTime >= projectileResetDelay)
    {
        projectilesCount++;
        projectileAccumulatedTime = 0.f;
    }
}

void Player::DecreaseCoins()
{
    coinsCollected--;
}

void Player::HandleCoinLifeExchange(bool exchangeCoins)
{
    const bool canExchangeCoins = exchangeCoins && !coinsExchanged && coinsCollected > 0 && health < maxHealth;
    const bool shouldResetExchange = !exchangeCoins && coinsExchanged;

    if (canExchangeCoins)
    {
        DecreaseCoins();
        IncreaseHealth();
        coinsExchanged = true;
    }
    else if (shouldResetExchange)
    {
        coinsExchanged = false;
    }
}

sf::Vector2f Player::GetVelocity() const
{
    return velocity;
}

void Player::StartBlinking()
{
    SetState(State::Blinking);
    blinkingTimer.restart();
    loseLifeCooldown.restart();
}

sf::Vector2f Player::GetPosition() const
{
    return position;
}
