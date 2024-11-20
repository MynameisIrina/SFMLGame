#include "Background.h"
#include <iostream>

Background::Background(Player& pl) : player(pl) {}

void Background::Initialize(sf::RenderWindow& window)
{
    // Set background image
    texture.loadFromFile("Assets/Background/Background.png");
    textureSize = texture.getSize();
    windowSize = window.getSize();
    // scale the background image to the window size
    float ScaleX = (float) windowSize.x / textureSize.x;
    float ScaleY = (float) windowSize.y / textureSize.y;
    sprite1.setTexture(texture);
    sprite1.setScale(ScaleX, ScaleY);
    sprite2.setTexture(texture);
    sprite2.setScale(ScaleX, ScaleY);
    sprite2.setPosition(windowSize.x, 0);
}

void Background::Update(float& dt)
{
    if(player.isMovingRight){

        sprite1.move(-80.f * dt, 0);
        sprite2.move(-80.f * dt, 0);

        if(sprite1.getPosition().x + sprite1.getGlobalBounds().width <= 0){
            sprite1.setPosition(sprite2.getPosition().x + sprite2.getGlobalBounds().width, 0);
        }
        if(sprite2.getPosition().x + sprite2.getGlobalBounds().width <= 0){
            sprite2.setPosition(sprite1.getPosition().x + sprite1.getGlobalBounds().width, 0);
        }
    }
}


void Background::Draw(sf::RenderTarget& rt) const
{
    rt.draw(sprite1);
    rt.draw(sprite2);
}

sf::Sprite Background::GetSprite1() const
{
    return sprite1;
}

sf::Sprite Background::GetSprite2() const
{
    return sprite2;
}
