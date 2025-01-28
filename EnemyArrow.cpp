#include "EnemyArrow.h"
#include "Utilities.h"
#include "Projectile.h"

EnemyArrow::EnemyArrow(ArrowPool arrowPool) : arrowPool(std::move(arrowPool)) {}

void EnemyArrow::Initialize(sf::Sprite &sprite, sf::Vector2f startPosition, int health, int damage)
{
    this->sprite = sprite;
    this->position = startPosition;
    this->health = health;
    this->damage = damage;

    this->sprite.setOrigin(this->sprite.getLocalBounds().width / 2, this->sprite.getLocalBounds().height / 2);
    this->sprite.setScale(-1.f * scale, 1.f * scale);

    boundingBox = Utilities::CreateBoundingBox(this->sprite, this->position);
}

void EnemyArrow::Update(const std::shared_ptr<Player> &player, const std::shared_ptr<Camera> &camera, float dt)
{
    Enemy::Update(player, camera, dt);
    HandleShooting(camera);
    HandleCollision(player);
    HandleRotation(player);
    UpdateAnimation(dt);
    UpdateView();
    arrowPool.Update(player, camera, dt);
}

void EnemyArrow::UpdateAnimation(float dt)
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
    bool enemyWithinCamera = camera->CalculateRightBound() >= position.x;

    if (enemyWithinCamera && !isShooting)
    {
        if (currentAnim == shootingFrame)
        {
            isShooting = true;
            ShootArrow();
        }
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
    std::vector<Projectile *> activeProjectiles = player->GetActiveProjectiles();

    for (const auto &activeProjectile : activeProjectiles)
    {
        if (activeProjectile != nullptr)
        {
            bool collision = Math::CheckRectCollision(activeProjectile->circle.getGlobalBounds(), boundingBox.getGlobalBounds());

            if (collision)
            {
                activeProjectile->isActive = false;
                Enemy::TakeDamage(damage);
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
        arrow->position = facingLeft ? sf::Vector2f(position.x - shootingOffset, position.y) : sf::Vector2f(position.x + shootingOffset, position.y);
        arrow->sprite.setScale(facingLeft ? arrow->sprite.getScale().x : -1 * arrow->sprite.getScale().x, arrow->sprite.getScale().y);
        arrow->velocity = facingLeft ? velocity : -velocity;
    }
}

void EnemyArrow::Draw(const std::shared_ptr<sf::RenderWindow> window) const
{
    Enemy::Draw(window);

    arrowPool.Draw(window);
    window->draw(boundingBox);
}
