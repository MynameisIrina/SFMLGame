#include "Arrow.h"

Arrow::Arrow(const sf::Sprite &sprite) : sprite(sprite)
{
    this->sprite.setScale(-1 * sprite.getScale().x, 1 * sprite.getScale().y);
    this->sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
    boundingBox = CreateBoundingBox();
}

void Arrow::Update(const std::shared_ptr<Player> &player, const std::shared_ptr<Camera> camera, float dt)
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

sf::RectangleShape Arrow::CreateBoundingBox()
{
    boundingBox.setSize(sf::Vector2f(sprite.getGlobalBounds().getSize().x, sprite.getGlobalBounds().getSize().y));
    boundingBox.setFillColor(sf::Color::Transparent);
    boundingBox.setOutlineColor(sf::Color::White);
    boundingBox.setOutlineThickness(1);
    boundingBox.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
    boundingBox.setPosition(position);
    return boundingBox;
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
