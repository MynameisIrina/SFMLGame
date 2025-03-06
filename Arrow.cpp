#include "Arrow.h"
#include "Utilities.h"
#include "Math.h"
#include "Player.h"
#include "Camera.h"

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

    UpdateView();
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