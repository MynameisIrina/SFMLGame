#include "EnemyArrow.h"
#include "Utilities.h"
#include "Projectile.h"
#include "Coin.h"
#include "Collectible.h"

EnemyArrow::EnemyArrow(const std::shared_ptr<ArrowPool>& arrowPool, const std::shared_ptr<AudioManager>& audioManager) : arrowPool(arrowPool), Enemy(audioManager){}

void EnemyArrow::Initialize(const sf::Sprite &sprite, const sf::Vector2f& position, const int health, const int damage)
{
    this->sprite = sprite;
    this->position = position;
    this->startPosition = position;
    this->health = health;
    this->damage = damage;
    this->maxHealth = health;
    shouldBeRespawned = true;

    const auto &bounds = this->sprite.getLocalBounds();
    this->sprite.setOrigin(bounds.width * 0.5f, bounds.height * 0.5f);
    this->sprite.setScale(-1.f * scale, scale);

    boundingBox = Utilities::CreateBoundingBox(this->sprite, this->position);
    initialBoundingBox = boundingBox;
    
}

void EnemyArrow::Update(const std::shared_ptr<Player> &player, const std::shared_ptr<Camera> &camera, const float dt)
{
    if (state == State::Dead)
        return;

    Enemy::Update(player, camera, dt);
    HandleShooting(camera);
    HandleCollision(player);
    HandleRotation(player);
    UpdateAnimation(dt);
    UpdateView();
}

void EnemyArrow::UpdateAnimation(const float dt)
{
    if (state == State::Dead) return;

    animationTimer += dt;
    if (animationTimer >= animationInterval)
    {
        if (currentAnim >= maxFrames)
        {
            currentAnim = 0;
        }

        isShooting = false;

        currentAnim++;
        animationTimer = 0.f;
    }
}

void EnemyArrow::UpdateView()
{
    if (state == State::Dead) return;

    sprite.setTextureRect(sf::IntRect(currentAnim * frameSize, 0, frameSize, frameSize));
    sprite.setPosition(position);
    boundingBox.setPosition(position);
}

void EnemyArrow::HandleShooting(const std::shared_ptr<Camera>& camera)
{
    if (state == State::Dead) return;

    const bool enemyWithinCamera = camera->CalculateRightBound() >= (position.x - boundingBox.getSize().x * 0.5f) && camera->CalculateLeftBound() <= (position.x + boundingBox.getSize().x * 0.5f);

    if (enemyWithinCamera && !isShooting && (currentAnim == shootingFrame))
    {
        isShooting = true;
        ShootArrow();
    }
}

void EnemyArrow::HandleRotation(const std::shared_ptr<Player> &player)
{
    if (state == State::Dead) return;

    if (player->GetPosition().x > position.x)
    {
        sprite.setScale(1.f * scale, 1.f * scale);
    }
    else
    {
        sprite.setScale(-1.f * scale, 1.f * scale);
    }
}

void EnemyArrow::HandleCollision(const std::shared_ptr<Player> &player)
{
    if (state == State::Dead) return;

    const auto &activeProjectiles = player->GetActiveProjectiles();
    const auto &enemyBounds = boundingBox.getGlobalBounds();

    for (const auto &activeProjectile : activeProjectiles)
    {
        if (activeProjectile != nullptr && activeProjectile->isActive)
        {
            bool collision = Math::CheckRectCollision(activeProjectile->GetBoundingCircle().getGlobalBounds(), enemyBounds);

            if (collision)
            {
                activeProjectile->isActive = false;
                Enemy::TakeDamage(damage);
                break;
            }
        }
    }
}

void EnemyArrow::HandleDeath(const std::shared_ptr<CollectibleManager>& collectibleManager)
{
    if (state != State::Dead || handledDeath) {
        return;
    }

    handledDeath = true;
    boundingBox = sf::RectangleShape();
    
    auto collectible = collectibleManager->CreateCoin(position);
    collectibleManager->AddCollectible(std::move(collectible));
    
    audioManager->PlaySound("kill enemy");
}


Enemy::State EnemyArrow::GetState()
{
    return state;
}

void EnemyArrow::ClearArrowPool()
{
    arrowPool->Clear();
}

void EnemyArrow::HandleFlyingArrows(const std::shared_ptr<Player> &player, const std::shared_ptr<Camera> &camera, const float dt)
{
    arrowPool->Update(player, camera, dt);
}

void EnemyArrow::ShootArrow()
{
    Arrow *arrow = arrowPool->GetArrow();

    if (arrow != nullptr)
    {
        bool facingLeft = this->sprite.getScale().x < 0;
        const float xOffset = facingLeft ? -shootingOffset : shootingOffset;
        arrow->position = sf::Vector2f(position.x + xOffset, position.y);
        arrow->sprite.setScale(facingLeft ? arrow->sprite.getScale().x
                                          : -arrow->sprite.getScale().x,
                               arrow->sprite.getScale().y);
        arrow->velocity = velocity;
        arrow->direction = facingLeft ? 1.0f : -1.0f;
    }
}

void EnemyArrow::Draw(const std::shared_ptr<sf::RenderWindow>& window) const
{
    arrowPool->Draw(window);

    if (state == State::Dead)
        return;

    Enemy::Draw(window);
    window->draw(boundingBox);

}