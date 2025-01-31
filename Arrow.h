#pragma once
#include <SFML/Graphics.hpp>
#include "TextureLoader.h"
#include "Player.h"

class Arrow
{

public:
    Arrow(sf::Sprite& sprite);
    void Update(const std::shared_ptr<Player>& player,const std::shared_ptr<Camera>& camera, const float dt);
    sf::RectangleShape GetBoundingBox() const;
    void HandleMovement(const std::shared_ptr<Player> &player, const std::shared_ptr<Camera>& camera, const float dt);
    void UpdateView();
    sf::Sprite sprite;
    bool isActive = false;
    sf::Vector2f position;
    float velocity;
    bool recentlyDeactivated;

private:
    sf::RectangleShape boundingBox;
};
