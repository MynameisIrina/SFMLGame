#include "Eagle.h"
#include "Utilities.h"

Eagle::Eagle(const std::shared_ptr<AudioManager> &audioManager) : Enemy(audioManager) {}

void Eagle::Initialize(const sf::Sprite &sprite, const sf::Vector2f &position, const int health, const int damage)
{
    this->position = position;
    this->sprite = sprite;
    this->health = health;
    this->damage = damage;
    shouldBeRespawned = false;

    const auto &bounds = this->sprite.getLocalBounds();
    this->sprite.setOrigin(bounds.width * 0.5f, bounds.height * 0.5f);
    this->sprite.setScale(scale, scale);

    boundingBox = Utilities::CreateBoundingBox(this->sprite, this->position);
    boundingBox.setSize(boundingBox.getSize() * scalar);
    initialBoundingBox = boundingBox;
}

void Eagle::Update(const std::shared_ptr<Player> &player, const std::shared_ptr<Camera> &camera, const float dt)
{
    if (state == Enemy::State::Dead)
        return;

    Enemy::Update(player, camera, dt);
    HandleHorizontalMovement(camera, dt);
    HandleCollision(player);
    UpdateAnimation(dt);
    UpdateView();
}

void Eagle::HandleDeath(const std::shared_ptr<CollectibleManager> &collectibleManager)
{
    if (state != Enemy::State::Dead || handledDeath)
        return;

    handledDeath = true;
    boundingBox = sf::RectangleShape();

    audioManager->PlaySound("kill enemy");
}

void Eagle::UpdateAnimation(const float dt)
{
    if (state == Enemy::State::Dead)
        return;

    animationTimer += dt;
    if (animationTimer >= animationInterval)
    {
        if (currentAnim >= maxFrames)
        {
            currentAnim = 0;
        }

        currentAnim++;
        animationTimer = 0.f;
    }
}

void Eagle::HandleCollision(const std::shared_ptr<Player> &player)
{
    if (state == Enemy::State::Dead)
        return;

    const auto &activeProjectiles = player->GetActiveProjectiles();
    const auto &enemyBounds = boundingBox.getGlobalBounds();

    for (const auto &activeProjectile : activeProjectiles)
    {
        if (activeProjectile != nullptr && activeProjectile->isActive)
        {
            bool collision = Math::CheckRectCollision(activeProjectile->GetBoundingCircle().getGlobalBounds(), enemyBounds);

            if (collision)
            {
                activeProjectile->isActive = false;
                Enemy::TakeDamage(damage);
                break;
            }
        }
    }
}

sf::Vector2f Eagle::GetPosition() const
{
    return position;
}

int Eagle::GetDirection() const
{
    return movementDirection;
}

void Eagle::UpdateView()
{
    if (state == Enemy::State::Dead)
        return;

    sprite.setTextureRect(sf::IntRect(currentAnim * TextureLoader::rectWidthEagel + TextureLoader::eageloffsetX, 0, TextureLoader::rectWidthEagel, TextureLoader::rectHeightEagel));
    sprite.setPosition(position);
    boundingBox.setPosition(position.x, position.y + boundingBoxOffsetY);
}

void Eagle::Draw(const std::shared_ptr<sf::RenderWindow> &window) const
{
    if (state == Enemy::State::Dead) return;

    Enemy::Draw(window);
    window->draw(boundingBox);
}

void Eagle::SetMovementDirection(const int direction)
{
    movementDirection = direction;
    sprite.setScale(-1 * (sprite.getScale().x * movementDirection), sprite.getScale().y);
}

void Eagle::HandleHorizontalMovement(const std::shared_ptr<Camera> &camera, const float dt)
{
    if (state == Enemy::State::Dead)
        return;

    position.x += speed * movementDirection * dt;
}
