#include "enemies/enemyarrow/projectiles/Arrow.h"
#include "utilities/Utilities.h"
#include "utilities/Math.h"
#include "player/Player.h"
#include "camera/Camera.h"

Arrow::Arrow(const sf::Sprite &sprite) : sprite(sprite)
{
    this->sprite.setScale(-1 * sprite.getScale().x, 1 * sprite.getScale().y);
    this->sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
    boundingBox = Utilities::CreateBoundingBox(sprite, sf::Vector2f{0, 0});
    recentlyDeactivated = false;
}

void Arrow::Update(const std::shared_ptr<Player> &player, const std::shared_ptr<Camera> &camera, const float dt)
{
    if (!IsArrowActive())
        return;

    position.x += velocity * direction * dt;
    UpdateView();

    HandleCollision(player,camera);
}

sf::RectangleShape Arrow::GetBoundingBox() const
{
    return boundingBox;
}

void Arrow::UpdateView()
{
    sprite.setPosition(position);
    boundingBox.setPosition(sprite.getPosition());
}

bool Arrow::IsArrowActive()
{
    if (!isActive)
    {
        if (recentlyDeactivated)
        {
            recentlyDeactivated = false;
        }

        return false;
    }

    return true;
}

void Arrow::HandleCollision(const std::shared_ptr<Player> &player, const std::shared_ptr<Camera> &camera)
{

    const bool outOfCameraBounds = position.x < camera->CalculateLeftBound() || position.x > camera->CalculateRightBound();
    const bool collisionWithPlayer = Math::CheckRectCollision(player->GetBoundingBox().getGlobalBounds(), boundingBox.getGlobalBounds());

    if (outOfCameraBounds || collisionWithPlayer)
    {
        isActive = false;
        recentlyDeactivated = true;

        if (collisionWithPlayer)
        {
            player->StartBlinking();
            player->DecreaseHealth();
        }
    }
}
