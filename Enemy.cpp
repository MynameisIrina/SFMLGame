#include "Enemy.h"

Enemy::Enemy(){}

void Enemy::Update(const std::shared_ptr<Player>& player, const std::shared_ptr<Camera>& camera, float dt)
{
}

void Enemy::TakeDamage(int amount)
{
    health -= amount;
    if (health <= 0)
    {
        isDead = true;
    }
}

sf::RectangleShape Enemy::GetBoundingBox()
{
    return boundingBox;
}

void Enemy::Draw(const std::shared_ptr<sf::RenderWindow> window) const
{
    window->draw(sprite);
}
