#include "Background.h"
#include <iostream>
#include <list>

std::shared_ptr<TextureLoader> Background::txLoader = nullptr; 

Background::Background(std::shared_ptr<TextureLoader> txLoaderRef)
{
    txLoader = txLoaderRef;
}

void Background::Initialize(const std::shared_ptr<sf::RenderWindow> window)
{
    textureSize = txLoader->GetTexture(TextureLoader::Background).getSize();
    windowSize = window->getSize();
    scaleX = (float)windowSize.x / textureSize.x;
    scaleY = (float)windowSize.y / textureSize.y;

    AddNewBackgroundSprite(0);
    AddNewBackgroundSprite(windowSize.x);
}


void Background::GenerateNewSprite(const std::shared_ptr<Player> player)
{
    sf::Vector2f playerPosition = player->GetPosition();
    
    if (playerPosition.x > spriteList.back().getPosition().x)
    {
        AddNewBackgroundSprite(spriteList.back().getPosition().x + windowSize.x);
    }

}

void Background::AddNewBackgroundSprite(float positionX)
{
    sf::Sprite newSprite = txLoader->SetSprite(TextureLoader::TextureType::Background);;
    newSprite.setScale(scaleX, scaleY);
    newSprite.setPosition(positionX, 0);
    spriteList.push_back(newSprite);
}


void Background::Draw(std::shared_ptr<sf::RenderTarget> rt) const
{
    for (const auto& sprite : spriteList)
    {
        rt->draw(sprite);
    }
}
