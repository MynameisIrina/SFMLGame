// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "Arrow.h"
#include "Utilities.h"

Arrow::Arrow(sf::Sprite &sprite) : sprite(sprite)
{
    this->sprite.setScale(-1 * sprite.getScale().x, 1 * sprite.getScale().y);
    this->sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
    boundingBox = Utilities::CreateBoundingBox(sprite, sf::Vector2f{0, 0});
    velocity = 0;
    recentlyDeactivated = false;
}

void Arrow::Update(const std::shared_ptr<Player> &player, const std::shared_ptr<Camera> &camera, const float dt)
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

    position.x -= velocity * dt;

    const bool outOfCameraBounds = sprite.getScale().x < 0 ? position.x < camera->CalculateLeftBound() : position.x > camera->CalculateRightBound();
    const bool collisionWithPlayer = Math::CheckRectCollision(player->GetBoundingBox().getGlobalBounds(), boundingBox.getGlobalBounds());

    if (outOfCameraBounds || collisionWithPlayer)
    {
        isActive = false;
        recentlyDeactivated = true;

        if (collisionWithPlayer)
        {
            player->SetState(Player::State::Blinking);
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
    boundingBox.setPosition(position);
}
