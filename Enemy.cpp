#include "Enemy.h"

Enemy::Enemy() { state = Alive; }

void Enemy::Update(const std::shared_ptr<Player>& player, const std::shared_ptr<Camera>& camera, float dt)
{
    if(state == Dead) return;
}

void Enemy::TakeDamage(const int amount)
{
    if(state == Dead) return;

    health -= amount;
    if (health <= 0)
    {
        state = Dead;
        health = 0;
    }
}

Enemy::State Enemy::GetState() const
{
    return state;
}

sf::RectangleShape Enemy::GetBoundingBox() const
{
    return boundingBox;
}

void Enemy::Draw(const std::shared_ptr<sf::RenderWindow>& window) const
{
    if(state == Dead) return;
    window->draw(sprite);
}