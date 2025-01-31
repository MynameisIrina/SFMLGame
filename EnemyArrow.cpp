#include "EnemyArrow.h"
#include "Utilities.h"
#include "Projectile.h"

EnemyArrow::EnemyArrow(ArrowPool arrowPool) : arrowPool(std::move(arrowPool)) {}

void EnemyArrow::Initialize(const sf::Sprite &sprite, const sf::Vector2f startPosition, const int health, const int damage)
{
    this->sprite = sprite;
    this->position = startPosition;
    this->health = health;
    this->damage = damage;

    const auto &bounds = this->sprite.getLocalBounds();
    this->sprite.setOrigin(bounds.width * 0.5f, bounds.height * 0.5f);
    this->sprite.setScale(-1.f * scale, scale);

    boundingBox = Utilities::CreateBoundingBox(this->sprite, this->position);
}

void EnemyArrow::Update(const std::shared_ptr<Player> &player, const std::shared_ptr<Camera> &camera, float dt)
{
    if (state == State::Dead)
        return;

    Enemy::Update(player, camera, dt);
    HandleShooting(camera);
    HandleCollision(player);
    HandleRotation(player);
    UpdateAnimation(dt);
    UpdateView();
    arrowPool.Update(player, camera, dt);
}

void EnemyArrow::UpdateAnimation(const float dt)
{
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
    sprite.setTextureRect(sf::IntRect(currentAnim * frameSize, 0, frameSize, frameSize));
    sprite.setPosition(position);
    boundingBox.setPosition(position);
}

void EnemyArrow::HandleShooting(const std::shared_ptr<Camera> camera)
{
    const bool enemyWithinCamera = camera->CalculateRightBound() >= position.x;

    if (enemyWithinCamera && !isShooting && (currentAnim == shootingFrame))
    {
        isShooting = true;
        ShootArrow();
    }
}

void EnemyArrow::HandleRotation(const std::shared_ptr<Player> &player)
{
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
    const auto &activeProjectiles = player->GetActiveProjectiles();
    const auto &enemyBounds = boundingBox.getGlobalBounds();

    for (const auto &activeProjectile : activeProjectiles)
    {
        if (activeProjectile != nullptr && activeProjectile->isActive)
        {
            bool collision = Math::CheckRectCollision(activeProjectile->circle.getGlobalBounds(), enemyBounds);

            if (collision)
            {
                activeProjectile->isActive = false;
                Enemy::TakeDamage(damage);
                break;
            }
        }
    }
}

void EnemyArrow::HandleDeath()
{
}

Enemy::State EnemyArrow::GetState()
{
    return state;
}

void EnemyArrow::ShootArrow()
{
    Arrow *arrow = arrowPool.GetArrow();

    if (arrow != nullptr)
    {
        bool facingLeft = this->sprite.getScale().x < 0;
        const float xOffset = facingLeft ? -shootingOffset : shootingOffset;
        arrow->position = sf::Vector2f(position.x + xOffset, position.y);
        arrow->sprite.setScale(facingLeft ? arrow->sprite.getScale().x
                                          : -arrow->sprite.getScale().x,
                               arrow->sprite.getScale().y);
        arrow->velocity = facingLeft ? velocity : -velocity;
    }
}

void EnemyArrow::Draw(const std::shared_ptr<sf::RenderWindow> window) const
{
    if (state == State::Dead)
        return;

    Enemy::Draw(window);
    arrowPool.Draw(window);
    window->draw(boundingBox);
}
