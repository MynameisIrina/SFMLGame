#pragma once
#include <SFML/Graphics.hpp>
#include "TextureLoader.h"
#include "Player.h"
#include "Math.h"

class Arrow
{

public:
    Arrow(sf::Sprite& sprite);
    void Update(const std::shared_ptr<Player>& player,const std::shared_ptr<Camera>& camera, float dt);
    sf::RectangleShape GetBoundingBox();
    void UpdateView();
    sf::Sprite GetSprite();
    bool isActive = false;
    sf::Vector2f position;
    float velocity;
    bool recentlyDeactivated;

private:
    sf::RectangleShape boundingBox;
    sf::Sprite sprite;
};
