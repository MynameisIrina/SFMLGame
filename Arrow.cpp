#include "Arrow.h"

Arrow::Arrow(const sf::Sprite& sprite): sprite(sprite)
{
    this->sprite.setScale(-1 * sprite.getScale().x, 1 * sprite.getScale().y);
}

void Arrow::Update(const std::shared_ptr<Player>& player, float dt)
{
    if(isActive)
    {
        position.x -= velocity * dt;

        if(position.x < player->GetPosition().x)
        {
            isActive = false;
        }
    }

    UpdateView();
}

void Arrow::UpdateView()
{
    sprite.setPosition(position);
}

sf::Sprite Arrow::GetSprite()
{
    return sprite;
}
