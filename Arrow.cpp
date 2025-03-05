#include "Arrow.h"
#include "Utilities.h"

Arrow::Arrow(sf::Sprite &sprite) : sprite(sprite)
{
    this->sprite.setScale(-1 * sprite.getScale().x, 1 * sprite.getScale().y);
    this->sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
    boundingBox = Utilities::CreateBoundingBox(sprite, sf::Vector2f{0, 0});
    velocity = 100.f;
    recentlyDeactivated = false;
}

void Arrow::Update(const std::shared_ptr<Player> &player, const std::shared_ptr<Camera> &camera, const float dt)
{
    if (!IsArrowActive())
        return;

    // Store previous position before updating
    //previousPos = position;

    position.x += velocity * direction * dt;
    
    sf::Vector2f curPos = position;

    // // Output velocity every 2 seconds
    // static float timeSinceLastOutput = 0.0f;
    // if ((timeSinceLastOutput += dt) >= 2.0f)
    // {
    //     std::cout << (position.x - previousPos.x) / dt << std::endl;
    //     timeSinceLastOutput = 0.0f;
    // }

    const bool outOfCameraBounds = position.x < camera->CalculateLeftBound() || position.x > camera->CalculateRightBound();
    const bool collisionWithPlayer = Math::CheckRectCollision(player->GetBoundingBox().getGlobalBounds(), boundingBox.getGlobalBounds());

    if (outOfCameraBounds || collisionWithPlayer)
    {
        isActive = false;
        recentlyDeactivated = true;

        if (collisionWithPlayer)
        {
            // player->SetState(Player::State::Blinking);
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
