#include "Enemy.h"
#include <chrono>
#include <thread>

Enemy::Enemy(const std::shared_ptr<AudioManager> &audioManager) : audioManager(audioManager)
{
    state = Alive;
}

void Enemy::Update(const std::shared_ptr<Player> &player, const std::shared_ptr<Camera> &camera, float dt)
{
    if (state == Dead)
        return;

    if (takingDamage && damageTimer.getElapsedTime().asSeconds() >= takingDamageAnimationDuration)
    {
        sprite.setColor(sf::Color::White);
        takingDamage = false;
    }
}

void Enemy::Respawn()
{
    if(!shouldBeRespawned) return;
    
    state = Alive;
    position = startPosition;
    boundingBox = initialBoundingBox;
    handledDeath = false;
    health = maxHealth;
}

void Enemy::TakeDamage(const int amount)
{
    if (state == Dead)
        return;

    health -= amount;
    ShowDamageAnimation();

    if (health <= 0)
    {
        state = Dead;
        health = 0;
    }
}

void Enemy::ShowDamageAnimation()
{
    if (state == Dead)
        return;

    takingDamage = true;
    sprite.setColor(sf::Color::Red);
    damageTimer.restart();
}

Enemy::State Enemy::GetState() const
{
    return state;
}

sf::RectangleShape Enemy::GetBoundingBox() const
{
    return boundingBox;
}

void Enemy::Draw(const std::shared_ptr<sf::RenderWindow> &window) const
{
    if (state == Dead)
        return;
        
    window->draw(sprite);
}

sf::Vector2f Enemy::GetPosition()
{
    return position;
}
