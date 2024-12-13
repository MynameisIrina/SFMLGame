#include "Background.h"
#include <iostream>
#include <list>

Background::Background(const Player &pl) : player(pl) {}

void Background::Initialize(const sf::RenderWindow &window)
{
    texture.loadFromFile("SFMLGame/Assets/Background/Background.png");
    textureSize = texture.getSize();
    windowSize = window.getSize();
    float ScaleX = (float)windowSize.x / textureSize.x;
    float ScaleY = (float)windowSize.y / textureSize.y;
    sprite1.setTexture(texture);
    sprite1.setScale(ScaleX, ScaleY);
    sprite1.setPosition(-400, 0);
    sprite2.setTexture(texture);
    sprite2.setScale(ScaleX, ScaleY);
    sprite2.setPosition(windowSize.x - 400, 0);

    positionSprite1 = sprite1.getPosition();
    positionSprite2 = sprite2.getPosition();
    currentSprite = sprite1;
}

void Background::Move(float dt)
{
    sf::Vector2f playerPosition = player.GetPosition();
    int deltaX = static_cast<int>(playerPosition.x) % windowSize.x;

    if(deltaX == 0)
    {
        if(playerPosition.x > sprite1.getPosition().x + windowSize.x){

            positionSprite1.x = positionSprite1.x + offsetPlacement;
        }
        else if(playerPosition.x > sprite2.getPosition().x + windowSize.x)
        {
            positionSprite2.x = positionSprite2.x + offsetPlacement;
        }
       
    }
}

void Background::UpdateView()
{
    sprite1.setPosition(positionSprite1);
    sprite2.setPosition(positionSprite2);
}

void Background::Draw(sf::RenderTarget &rt) const
{
    rt.draw(sprite1);
    rt.draw(sprite2);
}
