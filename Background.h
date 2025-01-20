#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include <list>

class Background
{
public:
    Background(std::shared_ptr<TextureLoader> txLoaderRef);
    void Initialize(const std::shared_ptr<sf::RenderWindow>  window);
    void Draw(std::shared_ptr<sf::RenderTarget> rt) const;
    void GenerateNewSprite(const std::shared_ptr<Player> player);
    void AddNewBackgroundSprite(float positionX);

private:
    static std::shared_ptr<TextureLoader> txLoader;
    float previousPlayerX;
    sf::Vector2f positionSprite1, positionSprite2, positionCurrentSprite;
    const float speed = 80.f;
    const float offsetPlacement = 1600.f;

    sf::Sprite sprite1, sprite2;
    std::list<sf::Sprite> spriteList;
    sf::Vector2u windowSize;
    sf::Vector2u textureSize;
    float scaleX, scaleY;
};