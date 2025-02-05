#include "Collectible.h"


void Collectible::Pickup(const std::shared_ptr<Player> &player)
{
}

void Collectible::Draw(const std::shared_ptr<sf::RenderWindow> window) const
{
    window->draw(sprite);
    window->draw(boundingBox);
}

void Collectible::Update(float dt)
{
}
