#include "Enemy.h"

Enemy::Enemy(){ state = Alive;}

void Enemy::Update(const std::shared_ptr<Player>& player, const std::shared_ptr<Camera>& camera, float dt)
{
}

void Enemy::TakeDamage(int amount)
{
    health -= amount;
    if (health <= 0)
    {
        state = Dead;
    }
}

Enemy::State Enemy::GetState()
{
    return state;
}

sf::RectangleShape Enemy::GetBoundingBox()
{
    return boundingBox;
}

void Enemy::Draw(const std::shared_ptr<sf::RenderWindow> window) const
{
    window->draw(sprite);
}
