#include "EnemyArrow.h"
#include "Utilities.h"

EnemyArrow::EnemyArrow(ArrowPool &&arrowPool) : arrowPool(std::move(arrowPool)) {}

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

void EnemyArrow::Update(const std::shared_ptr<Player>& player, const std::shared_ptr<Camera>& camera, float dt)
{
    Enemy::Update(player, camera, dt);
    HandleShooting(camera);
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

void EnemyArrow::ShootArrow()
{
    Arrow *arrow = arrowPool.GetArrow();
    if (arrow != nullptr)
    {
        arrow->position = sf::Vector2f(position.x - shootingOffset, position.y);
        arrow->velocity = velocity;
    }
}

void EnemyArrow::Draw(const std::shared_ptr<sf::RenderWindow> window) const
{
    Enemy::Draw(window);
    
    arrowPool.Draw(window);
    window->draw(boundingBox);
}
