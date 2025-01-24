#include "EnemyArrow.h"

EnemyArrow::EnemyArrow(ArrowPool &&arrowPool): arrowPool(std::move(arrowPool)) {}

void EnemyArrow::Initialize(sf::Sprite &sprite, sf::Vector2f startPosition, int health, int damage)
{
    //this->arrowPool = arrowPool;
    this->sprite = sprite;
    this->position = startPosition;
    this->health = health;
    this->damage = damage;
    this->sprite.setOrigin(this->sprite.getLocalBounds().width / 2, this->sprite.getLocalBounds().height / 2);
    this->sprite.setScale(- 1 * scale, 1 * scale);
}

void EnemyArrow::Update(const std::shared_ptr<Player> player, float dt)
{
    Enemy::Update(player, dt);
    UpdateAnimation(dt);
    UpdateView();
    arrowPool.Update(player, dt);
}

void EnemyArrow::UpdateAnimation(float dt)
{
    animationTimer += dt;
    if (animationTimer >= animationInterval)
    {
        if(currentAnim >= max_frames)
        {
            currentAnim = 0;
        }
        if(currentAnim == shooting_frame)
        {
            ShootArrow();
        }
        currentAnim++;
        animationTimer = 0.f;

    }
}

void EnemyArrow::UpdateView()
{
    sprite.setTextureRect(sf::IntRect(currentAnim * 128, 0 * 125, 125, 125));
    sprite.setPosition(position);
}

void EnemyArrow::ShootArrow()
{
    Arrow* arrow = arrowPool.GetArrow();
    arrow->position = sf::Vector2f(position.x - shootingOffset, position.y);
    arrow->velocity = velocity;
}

void EnemyArrow::Draw(const std::shared_ptr<sf::RenderWindow> window) const
{
    Enemy::Draw(window);
    arrowPool.Draw(window);
}
