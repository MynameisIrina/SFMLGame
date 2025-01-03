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

    sf::Texture grassTex;
    if (!grassTex.loadFromFile("SFMLGame/Assets/Background/PixelArtPlatformer/Texture/Ground.png"))
    {
        throw std::runtime_error("Failed to load grass texture");
    }
    textures[Tile_Grass] = grassTex;
}

sf::Texture& TextureLoader::GetTexture(TextureType type)
{
    return textures[type];
}

sf::Sprite TextureLoader::SetSprite(TextureType type)
{
    sf::Sprite sprite;
    sprite.setTexture(textures[type]);

    switch (type)
    {
    case Tile_Dirt:
        sprite.setTextureRect(sf::IntRect(SpriteCoordinates::groundX * TILE_SIZE, SpriteCoordinates::groundY * TILE_SIZE, TILE_SIZE, TILE_SIZE));
        break;
    case Tile_Grass:
        sprite.setTextureRect(sf::IntRect(SpriteCoordinates::grassX * TILE_SIZE, SpriteCoordinates::grassY * TILE_SIZE, TILE_SIZE, TILE_SIZE));
        break;
    }

    return sprite;
}
