#pragma once
#include "Player.h"

class Collectible
{
public:

    Collectible() = default;
    virtual ~Collectible() = default;

    virtual void Pickup(const std::shared_ptr<Player> &player);
    virtual void Draw(const std::shared_ptr<sf::RenderWindow>& window) const;
    virtual void Update(const std::shared_ptr<Player>& player, const float dt);

    bool  isPickedUp = false;
    
protected:
    sf::Sprite sprite;
    sf::Vector2f position;
    sf::RectangleShape boundingBox;
};