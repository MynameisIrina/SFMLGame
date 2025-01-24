#include "Background.h"
#include <iostream>
#include <list>


Background::Background(const std::shared_ptr<TextureLoader>& txLoaderRef)
{
    txLoader = txLoaderRef;
}

void Background::Initialize(const std::shared_ptr<sf::RenderWindow>& window)
{
    textureSize = txLoader->GetTexture(TextureLoader::Background).getSize();
    windowSize = window->getSize();

    AddNewBackgroundSprite(0, (float) windowSize.x / textureSize.x, (float) windowSize.y / textureSize.y);
    AddNewBackgroundSprite(windowSize.x, (float) windowSize.x / textureSize.x, (float) windowSize.y / textureSize.y);
}


void Background::GenerateNewSprite(const std::shared_ptr<Player>& player)
{
    const sf::Vector2f playerPosition = player->GetPosition();
    
    if (playerPosition.x > spriteList.back().getPosition().x)
    {
        AddNewBackgroundSprite(spriteList.back().getPosition().x + windowSize.x, (float) windowSize.x / textureSize.x, (float) windowSize.y / textureSize.y);
    }

}

void Background::AddNewBackgroundSprite(float x, float scaleX, float scaleY)
{
    sf::Sprite newSprite = txLoader->SetSprite(TextureLoader::TextureType::Background);;
    newSprite.setScale(scaleX, scaleY);
    newSprite.setPosition(x, 0);
    spriteList.push_back(newSprite);
}


void Background::Draw(const std::shared_ptr<sf::RenderTarget>& rt) const
{
    for (const auto& sprite : spriteList)
    {
        rt->draw(sprite);
    }
}
