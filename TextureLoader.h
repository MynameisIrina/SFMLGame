#pragma once
#include <SFML/Graphics.hpp>

class TextureLoader
{

public:
    enum TextureType
    {
        Tile_Grass,
        Tile_Dirt
    };

    enum SpriteCoordinates
    {
        groundX = 0,
        groundY = 10,
        grassX = 2,
        grassY = 8,
        tileSize = 32
    };

    void Initialize();
    sf::Texture& GetTexture(TextureType type);
    sf::Sprite SetSprite(TextureType type);

private:
    const int TILE_SIZE = 32;
    std::map <TextureType, sf::Texture> textures;
};