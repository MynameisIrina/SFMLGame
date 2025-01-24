#include "Enemy.h"

Enemy::Enemy(){}

void Enemy::Update(const std::shared_ptr<Player> player, float dt)
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


void Enemy::Draw(const std::shared_ptr<sf::RenderWindow> window) const
{
    window->draw(sprite);
}
