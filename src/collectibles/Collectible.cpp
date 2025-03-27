#include "collectibles/Collectible.h"
#include "player/Player.h"
#include "utilities/Math.h"


void Collectible::Pickup(const std::shared_ptr<Player> &player)
{
    bool collision = Math::CheckRectCollision(player->GetBoundingBox().getGlobalBounds(), boundingBox.getGlobalBounds());

    if(collision)
    {
        player->PickUpCoin();
        isPickedUp = true;
    }
}

void Collectible::Draw(const std::shared_ptr<sf::RenderWindow>& window) const
{
    window->draw(sprite);
}

void Collectible::Update(const std::shared_ptr<Player>& player, const float dt)
{
    Collectible::Pickup(player);
}
