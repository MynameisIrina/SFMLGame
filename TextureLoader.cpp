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
    textures[Tile_Dirt] = groundTex;
    textureCoordinates[Tile_Dirt] = sf::IntRect(SpriteCoordinates::groundX * TILE_SIZE, SpriteCoordinates::groundY * TILE_SIZE, TILE_SIZE, TILE_SIZE);

    sf::Texture grassTex;
    if (!grassTex.loadFromFile("SFMLGame/Assets/Background/PixelArtPlatformer/Texture/Ground.png"))
    {
        throw std::runtime_error("Failed to load grass texture");
    }
    textures[Tile_Grass] = grassTex;
    textureCoordinates[Tile_Grass] = sf::IntRect(SpriteCoordinates::grassX * TILE_SIZE, SpriteCoordinates::grassY * TILE_SIZE, TILE_SIZE, TILE_SIZE);

    sf::Texture playerTex;
    if (!playerTex.loadFromFile("SFMLGame/Assets/Player/Textures/Cat-Sheet.png"))
    {
        throw std::runtime_error("Failed to load player texture");
    }
    textures[Player] = playerTex;
    textureCoordinates[Player] = sf::IntRect(SpriteCoordinates::playerX * TILE_SIZE, SpriteCoordinates::playerY * TILE_SIZE, TILE_SIZE, TILE_SIZE);

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
    textureCoordinates[Obstacle] = sf::IntRect(SpriteCoordinates::obstacleX * TILE_SIZE, SpriteCoordinates::obstacleY * TILE_SIZE, TILE_SIZE, TILE_SIZE);

    sf::Texture healthBarTex;
    if (!healthBarTex.loadFromFile("SFMLGame/Assets/Background/PixelArtPlatformer/Texture/hearts.png"))
    {
        throw std::runtime_error("Failed to load lifebar texture");
    }
    textures[HealthBar] = healthBarTex;
    textureCoordinates[HealthBar] = sf::IntRect(SpriteCoordinates::HealthBarX * 16, SpriteCoordinates::HealthBarY * 16, 16, 16);

    sf::Texture enemyTex;
    if (!enemyTex.loadFromFile("SFMLGame/Assets/Background/PixelArtPlatformer/Texture/Enemy.png"))
    {
        throw std::runtime_error("Failed to load enemy texture");
    }
    textures[Enemy] = enemyTex;
    textureCoordinates[Enemy] = sf::IntRect(SpriteCoordinates::EnemyX * 125, SpriteCoordinates::EnemyY * 125, 125, 125);

    sf::Texture arrowTex;
    if (!arrowTex.loadFromFile("SFMLGame/Assets/Player/Textures/Arrow.png"))
    {
        throw std::runtime_error("Failed to load arrow texture");
    }
    textures[Arrow] = arrowTex;
    textureCoordinates[Arrow] = sf::IntRect(SpriteCoordinates::arrowX * 32, SpriteCoordinates::arrowY * 20, 16, 10);
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
    case Tile_Grass:
        return sf::Vector2i(grassX, grassY);
    case Tile_Dirt:
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
