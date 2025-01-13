#pragma once
#include <SFML/Graphics.hpp>

class TextureLoader
{

public:
    enum TextureType
    {
        Tile_Grass,
        Tile_Dirt,
        Player,
        Background,
        Obstacle,
        HealthBar
    };

    enum SpriteCoordinates
    {
        groundX = 0,
        groundY = 10,
        grassX = 2,
        grassY = 8,
        tileSize = 32,
        playerX = 0,
        playerY = 4,
        obstacleX = 27,
        obstacleY = 3,
        HealthBarX = 0,
        HealthBarY = 0
    };

    void Initialize();
    sf::Texture& GetTexture(TextureType type);
    //sf::Sprite& GetSprite(TextureType type);
    sf::Sprite SetSprite(TextureType type);

private:
    const int TILE_SIZE = 32;
    std::map <TextureType, sf::Texture> textures;
    std::map<TextureType, sf::IntRect> textureCoordinates;
};