#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"

class Background
{
public:
    Background(Player& pl);  
    void Initialize(sf::RenderWindow& window);
    void Update(float& dt);
    void Draw(sf::RenderTarget& rt) const;
    sf::Sprite GetSprite1() const;
    sf::Sprite GetSprite2() const;

private:
    Player& player;
    sf::Texture texture;
    sf::Sprite sprite1, sprite2;
    sf::Vector2u windowSize;
    sf::Vector2u textureSize;
};