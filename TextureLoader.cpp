#include "TextureLoader.h"
#include <stdexcept>
#include <iostream>

void TextureLoader::Initialize()
{
    sf::Texture groundTex;
    if (!groundTex.loadFromFile("Assets/Background/PixelArtPlatformer/Texture/Ground.png"))
    {
        throw std::runtime_error("Failed to load ground texture");
    }
    textures[TileDirt] = std::move(groundTex);
    textureCoordinates[TileDirt] = sf::IntRect(SpriteCoordinates::groundX * rectWidthDefault, SpriteCoordinates::groundY * rectHeightDefault, rectWidthDefault, rectHeightDefault);

    sf::Texture grassTex;
    if (!grassTex.loadFromFile("Assets/Background/PixelArtPlatformer/Texture/Ground.png"))
    {
        throw std::runtime_error("Failed to load grass texture");
    }
    textures[TileGrass] = std::move(grassTex);
    textureCoordinates[TileGrass] = sf::IntRect(SpriteCoordinates::grassX * rectWidthDefault, SpriteCoordinates::grassY * rectHeightDefault, rectWidthDefault, rectHeightDefault);

    sf::Texture playerTex;
    if (!playerTex.loadFromFile("Assets/Player/Textures/Cat-Sheet.png"))
    {
        throw std::runtime_error("Failed to load player texture");
    }
    textures[Player] = std::move(playerTex);
    textureCoordinates[Player] = sf::IntRect(playerX + playerOffsetX, playerY * rectHeightPlayer, rectWidthPlayer, rectHeightPlayer);

    sf::Texture backgroundTex;
    if (!backgroundTex.loadFromFile("Assets/Background/Background.png"))
    {
        throw std::runtime_error("Failed to load background texture");
    }
    textures[Background] = std::move(backgroundTex);

    sf::Texture obstacleTex;
    if (!obstacleTex.loadFromFile("Assets/Background/PixelArtPlatformer/Texture/VillageDetails.png"))
    {
        throw std::runtime_error("Failed to load obstacle texture");
    }
    textures[Obstacle] = std::move(obstacleTex);
    textureCoordinates[Obstacle] = sf::IntRect(SpriteCoordinates::obstacleX * rectWidthDefault, SpriteCoordinates::obstacleY * rectHeightDefault, rectWidthDefault, rectHeightDefault);

    sf::Texture healthBarTex;
    if (!healthBarTex.loadFromFile("Assets/UI/hearts.png"))
    {
        throw std::runtime_error("Failed to load lifebar texture");
    }
    textures[HealthBar] = std::move(healthBarTex);
    textureCoordinates[HealthBar] = sf::IntRect(SpriteCoordinates::HealthBarX * rectWidthHealthBar, SpriteCoordinates::HealthBarY * rectHeightHealthBar, rectWidthHealthBar, rectHeightHealthBar);

    sf::Texture enemyTex;
    if (!enemyTex.loadFromFile("Assets/Player/Textures/Enemy.png"))
    {
        throw std::runtime_error("Failed to load enemy texture");
    }
    textures[Enemy] = std::move(enemyTex);
    textureCoordinates[Enemy] = sf::IntRect(SpriteCoordinates::EnemyX, SpriteCoordinates::EnemyY, rectWidthEnemy, rectHeightEnemy);

    sf::Texture arrowTex;
    if (!arrowTex.loadFromFile("Assets/Player/Textures/Arrow.png"))
    {
        throw std::runtime_error("Failed to load arrow texture");
    }
    textures[Arrow] = std::move(arrowTex);    
    textureCoordinates[Arrow] = sf::IntRect(SpriteCoordinates::arrowX * rectWidthArrow , SpriteCoordinates::arrowY * rectHeightArrow , rectWidthArrow , rectHeightArrow);

    sf::Texture coinTex;
    if (!coinTex.loadFromFile("Assets/UI/coin_rotate.png"))
    {
        throw std::runtime_error("Failed to load coin texture");
    }
    textures[Coin] = std::move(coinTex);
    textureCoordinates[Coin] = sf::IntRect(SpriteCoordinates::coinX * rectWidthCoin + coinOffsetX , SpriteCoordinates::coinY * rectHeightCoin , rectWidthCoin , rectHeightCoin);

    sf::Texture treeTex;
    if (!treeTex.loadFromFile("Assets/Background/PixelArtPlatformer/Texture/VillageDetails.png"))
    {
        throw std::runtime_error("Failed to load tree texture");
    }
    textures[Tree] = std::move(treeTex);
    textureCoordinates[Tree] = sf::IntRect(SpriteCoordinates::treeX * rectWidthTree , SpriteCoordinates::treeY * rectHeightTree , rectWidthTree , rectHeightTree);
}

sf::Texture TextureLoader::GetTexture(TextureType type) const
{
    return textures.at(type);
}

sf::Sprite TextureLoader::SetSprite(TextureType type)
{
    sf::Sprite sprite;
    sprite.setTexture(textures[type]);
    if (type != Background)
    {
        sprite.setTextureRect(textureCoordinates[type]);
    }
    return sprite;
}

sf::Vector2i TextureLoader::GetSpriteCoordinates(TextureType type) const
{
    switch (type)
    {
    case TileGrass:
        return sf::Vector2i(grassX, grassY);
    case TileDirt:
        return sf::Vector2i(groundX, groundY);
    case Obstacle:
        return sf::Vector2i(obstacleX, obstacleY);
    case Player:
        return sf::Vector2i(playerX, playerY);
    case HealthBar:
        return sf::Vector2i(HealthBarX, HealthBarY);
    case Enemy:
        return sf::Vector2i(EnemyX, EnemyY);
    case Arrow:
        return sf::Vector2i(arrowX, arrowY);
    default:
        throw std::runtime_error("Invalid texture type");
    }
}
