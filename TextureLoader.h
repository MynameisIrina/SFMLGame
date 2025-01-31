#pragma once
#include <SFML/Graphics.hpp>

class TextureLoader
{

public:
    enum TextureType
    {
        TileGrass,
        TileDirt,
        Player,
        Background,
        Obstacle,
        HealthBar,
        Enemy,
        Arrow,
        ProjectileBar
    };

    enum SpriteCoordinates
    {
        groundX = 0,
        groundY = 10,
        grassX = 2,
        grassY = 8,
        playerX = 0,
        playerY = 9,
        obstacleX = 27,
        obstacleY = 3,
        HealthBarX = 0,
        HealthBarY = 0,
        EnemyX = 0,
        EnemyY = 0,
        arrowX = 2,
        arrowY = 0,
        projectileBarX = 1,
        projectileBarY = 0
    };

    

    void Initialize();
    sf::Texture GetTexture(TextureType type) const;
    sf::Sprite SetSprite(TextureType type);
    sf::Vector2i GetSpriteCoordinates(TextureType type) const;


    const int rectWidthDefault = 32;
    const int rectHeightDefault = 32;
    const int tileSizeHealthBar = 16;
    const int playerOffsetX = 4;
    const int rectWidthPlayer = 22;
    const int rectHeightPlayer = 16;
    const int rectWidthHealthBar = 16;
    const int rectHeightHealthBar = 16;
    const int rectWidthEnemy = 128;
    const int rectHeightEnemy = 128;
    const int rectWidthArrow = 16;
    const int rectHeightArrow = 10;
    const int rectWidthProjectileBar = 48;
    const int rectHeightProjectileBar = 12;

private:

    std::map <TextureType, sf::Texture> textures;
    std::map<TextureType, sf::IntRect> textureCoordinates;
};