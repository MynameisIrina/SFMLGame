#include "TextureLoader.h"
#include <stdexcept>
#include <iostream>

void TextureLoader::Initialize()
{
    sf::Texture groundTex;
    if (!groundTex.loadFromFile("SFMLGame/Assets/Background/PixelArtPlatformer/Texture/Ground.png"))
    {
        throw std::runtime_error("Failed to load ground texture");
    }
    textures[TileDirt] = groundTex;
    textureCoordinates[TileDirt] = sf::IntRect(SpriteCoordinates::groundX * rectWidthDefault, SpriteCoordinates::groundY * rectHeightDefault, rectWidthDefault, rectHeightDefault);

    sf::Texture grassTex;
    if (!grassTex.loadFromFile("SFMLGame/Assets/Background/PixelArtPlatformer/Texture/Ground.png"))
    {
        throw std::runtime_error("Failed to load grass texture");
    }
    textures[TileGrass] = grassTex;
    textureCoordinates[TileGrass] = sf::IntRect(SpriteCoordinates::grassX * rectWidthDefault, SpriteCoordinates::grassY * rectHeightDefault, rectWidthDefault, rectHeightDefault);

    sf::Texture playerTex;
    if (!playerTex.loadFromFile("SFMLGame/Assets/Player/Textures/Cat-Sheet.png"))
    {
        throw std::runtime_error("Failed to load player texture");
    }
    textures[Player] = playerTex;
    textureCoordinates[Player] = sf::IntRect(playerX + playerOffsetX, playerY * rectHeightPlayer, rectWidthPlayer, rectHeightPlayer);

    sf::Texture backgroundTex;
    if (!backgroundTex.loadFromFile("SFMLGame/Assets/Background/Background.png"))
    {
        throw std::runtime_error("Failed to load background texture");
    }
    textures[Background] = backgroundTex;

    sf::Texture obstacleTex;
    if (!obstacleTex.loadFromFile("SFMLGame/Assets/Background/PixelArtPlatformer/Texture/VillageDetails.png"))
    {
        throw std::runtime_error("Failed to load obstacle texture");
    }
    textures[Obstacle] = obstacleTex;
    textureCoordinates[Obstacle] = sf::IntRect(SpriteCoordinates::obstacleX * rectWidthDefault, SpriteCoordinates::obstacleY * rectHeightDefault, rectWidthDefault, rectHeightDefault);

    sf::Texture healthBarTex;
    if (!healthBarTex.loadFromFile("SFMLGame/Assets/Background/PixelArtPlatformer/Texture/hearts.png"))
    {
        throw std::runtime_error("Failed to load lifebar texture");
    }
    textures[HealthBar] = healthBarTex;
    textureCoordinates[HealthBar] = sf::IntRect(SpriteCoordinates::HealthBarX * rectWidthHealthBar, SpriteCoordinates::HealthBarY * rectHeightHealthBar, rectWidthHealthBar, rectHeightHealthBar);

    sf::Texture enemyTex;
    if (!enemyTex.loadFromFile("SFMLGame/Assets/Background/PixelArtPlatformer/Texture/Enemy.png"))
    {
        throw std::runtime_error("Failed to load enemy texture");
    }
    textures[Enemy] = enemyTex;
    textureCoordinates[Enemy] = sf::IntRect(SpriteCoordinates::EnemyX, SpriteCoordinates::EnemyY, rectWidthEnemy, rectHeightEnemy);

    sf::Texture arrowTex;
    if (!arrowTex.loadFromFile("SFMLGame/Assets/Player/Textures/Arrow.png"))
    {
        throw std::runtime_error("Failed to load arrow texture");
    }
    textures[Arrow] = arrowTex;
    textureCoordinates[Arrow] = sf::IntRect(SpriteCoordinates::arrowX * rectWidthArrow , SpriteCoordinates::arrowY * rectHeightArrow , rectWidthArrow , rectHeightArrow);

    sf::Texture coinTex;
    if (!coinTex.loadFromFile("SFMLGame/Assets/UI/coin_rotate.png"))
    {
        throw std::runtime_error("Failed to load coin texture");
    }
    textures[Coin] = coinTex;
    textureCoordinates[Coin] = sf::IntRect(SpriteCoordinates::coinX * rectWidthCoin + coinOffsetX , SpriteCoordinates::coinY * rectHeightCoin , rectWidthCoin , rectHeightCoin);
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
