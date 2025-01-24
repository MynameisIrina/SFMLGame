#pragma once
#include <SFML/Graphics.hpp>
#include "TextureLoader.h"
#include "Player.h"

class Arrow
{

public:
    Arrow(const sf::Sprite& sprite);
    void Update(const std::shared_ptr<Player>& player, float dt);
    void UpdateView();
    sf::Sprite GetSprite();
    bool isActive = false;
    sf::Vector2f position;
    sf::Sprite sprite;
    float velocity;
};
