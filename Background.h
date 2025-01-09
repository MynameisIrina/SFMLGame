#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include <list>

class Background
{
public:
    Background(const std::shared_ptr<Player>  pl, std::shared_ptr<TextureLoader> txLoaderRef);
    void Initialize(const std::shared_ptr<sf::RenderWindow>  window);
    void Update(float dt);
    void Draw(std::shared_ptr<sf::RenderTarget> rt) const;
    void GenerateNewSprite();
private:
    // model
    const std::shared_ptr<Player>  player;
    std::shared_ptr<TextureLoader> txLoader;
    float previousPlayerX;
    sf::Vector2f positionSprite1, positionSprite2, positionCurrentSprite;
    const float speed = 80.f;
    const float offsetPlacement = 1600.f;
    // view
    //sf::Texture texture;
    sf::Sprite sprite1, sprite2;
    std::list<sf::Sprite> spriteList;
    sf::Vector2u windowSize;
    sf::Vector2u textureSize;
    float scaleX, scaleY;
};