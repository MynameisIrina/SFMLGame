#include "Background.h"
#include <iostream>

Background::Background(const Player &pl) : player(pl) {}

void Background::Initialize(const sf::RenderWindow &window)
{
    // Set background image
    texture.loadFromFile("SFMLGame/Assets/Background/Background.png");
    textureSize = texture.getSize();
    windowSize = window.getSize();
    // scale the background image to the window size
    float ScaleX = (float)windowSize.x / textureSize.x;
    float ScaleY = (float)windowSize.y / textureSize.y;
    sprite1.setTexture(texture);
    sprite1.setScale(ScaleX, ScaleY);
    sprite1.setPosition(0,0);
    sprite2.setTexture(texture);
    sprite2.setScale(ScaleX, ScaleY);
    sprite2.setPosition(windowSize.x, 0);

    positionSprite1 = sprite1.getPosition();
    positionSprite2 = sprite2.getPosition();
}

void Background::Move(float dt)
{
    // calculaye how far the player moved right
    float deltaX = player.GetPosition().x - previousPlayerX;
    previousPlayerX = player.GetPosition().x;

    if (deltaX > 0.f)
    {
        positionSprite1.x -= 80.f * dt;
        positionSprite2.x -= 80.f * dt;
    }

}

void Background::UpdateView()
{
    sprite1.setPosition(positionSprite1);
    sprite2.setPosition(positionSprite2);

     if (sprite1.getPosition().x + sprite1.getGlobalBounds().width <= 0)
    {
        sprite1.setPosition(sprite2.getPosition().x + sprite2.getGlobalBounds().width, 0);
    }
    if (sprite2.getPosition().x + sprite2.getGlobalBounds().width <= 0)
    {
        sprite2.setPosition(sprite1.getPosition().x + sprite1.getGlobalBounds().width, 0);
    }
}

void Background::Draw(sf::RenderTarget &rt) const
{
    rt.draw(sprite1);
    rt.draw(sprite2);
}
