#include "EnemyArrow.h"

void EnemyArrow::Initialize(sf::Sprite &sprite, sf::Vector2f startPosition, int health, int damage)
{
    this->sprite = sprite;
    this->position = startPosition;
    this->health = health;
    this->damage = damage;
    this->sprite.setOrigin(this->sprite.getLocalBounds().width / 2, this->sprite.getLocalBounds().height / 2);
    this->sprite.setScale(0.45f, 0.45f);
}

void EnemyArrow::Update(const std::shared_ptr<Player> player, float dt)
{
    UpdateAnimation(dt);
    UpdateView();
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
        currentAnim++;
        animationTimer = 0.f;
    }
}

void EnemyArrow::UpdateView()
{
    sprite.setTextureRect(sf::IntRect(currentAnim * 128, 0 * 125, 125, 125));
    sprite.setPosition(position);
}
