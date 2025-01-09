#include "Background.h"
#include <iostream>
#include <list>

Background::Background(const std::shared_ptr<Player> pl, std::shared_ptr<TextureLoader> txLoaderRef) : player(pl), txLoader(txLoaderRef) {}

void Background::Initialize(const std::shared_ptr<sf::RenderWindow> window)
{
    textureSize = txLoader->GetTexture(TextureLoader::Background).getSize();
    windowSize = window->getSize();
    scaleX = (float)windowSize.x / textureSize.x;
    scaleY = (float)windowSize.y / textureSize.y;

    sprite1 = txLoader->SetSprite(TextureLoader::TextureType::Background);
    sprite1.setScale(scaleX, scaleY);
    sprite1.setPosition(0, 0);
    spriteList.push_back(sprite1);

    sprite2 = txLoader->SetSprite(TextureLoader::TextureType::Background);
    sprite2.setScale(scaleX, scaleY);
    sprite2.setPosition(windowSize.x, 0);
    spriteList.push_back(sprite2);
}

void Background::GenerateNewSprite()
{
    sf::Vector2f playerPosition = player->GetPosition();
    

    if (playerPosition.x > spriteList.back().getPosition().x)
    {
        sf::Sprite newSprite;
        newSprite.setTexture(txLoader->GetTexture(TextureLoader::Background));
        newSprite.setScale(scaleX, scaleY);
        newSprite.setPosition(spriteList.back().getPosition().x + windowSize.x, 0);
        spriteList.push_back(newSprite);
    }

}

void Background::Draw(std::shared_ptr<sf::RenderTarget> rt) const
{
    for (const auto& sprite : spriteList)
    {
        rt->draw(sprite);
    }
}
