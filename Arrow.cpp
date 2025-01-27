#include "Arrow.h"
#include "Utilities.h"

Arrow::Arrow(sf::Sprite &sprite) : sprite(sprite)
{
    this->sprite.setScale(-1 * sprite.getScale().x, 1 * sprite.getScale().y);
    this->sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
    boundingBox = Utilities::CreateBoundingBox(sprite, sf::Vector2f{0,0});
}

void Arrow::Update(const std::shared_ptr<Player> &player, const std::shared_ptr<Camera>& camera, float dt)
{
    // Allow reuse in the next frame
    // Otherwise the deactived arrow will be picked up again and will detect collision with player twice
    if (!isActive)
    {
        if (recentlyDeactivated)
        {
            recentlyDeactivated = false;
        }
        return;
    }

    if (isActive)
    {
        position.x -= velocity * dt;

        bool outOfCameraBounds = position.x < camera->CalculateLeftBound();
        bool collisionWithPlayer = Math::CheckRectCollision(player->GetBoundingBox().getGlobalBounds(), boundingBox.getGlobalBounds());


        if (outOfCameraBounds || collisionWithPlayer)
        {
            isActive = false;
            recentlyDeactivated = true;
        }


        if (collisionWithPlayer)
        {
            player->DecreaseHealth();
        }

    }

    UpdateView();
}

sf::RectangleShape Arrow::GetBoundingBox()
{
    return boundingBox;
}

void Arrow::UpdateView()
{
    sprite.setPosition(position);
    boundingBox.setPosition(position);
}

sf::Sprite Arrow::GetSprite()
{
    return sprite;
}
