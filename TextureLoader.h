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
        ProjectileBar,
        Coin,
        Tree,
        Eagel,
        Hairball
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
        projectileBarY = 0,
        coinX = 0,
        coinY = 0,
        treeX = 6,
        treeY = 3,
        eagelX = 0,
        eagelY = 0,
        hairballX = 0,
        hairballY = 0
    };

    

    void Initialize();
    sf::Texture GetTexture(TextureType type) const;
    sf::Sprite SetSprite(TextureType type);
    sf::Vector2i GetSpriteCoordinates(TextureType type) const;


    static const int rectWidthDefault = 32;
    static const int rectHeightDefault = 32;
    static const int tileSizeHealthBar = 16;
    static const int playerOffsetX = 4;
    static const int playerHeightOffset = 3;
    static const int rectWidthPlayer = 22;
    static const int rectHeightPlayer = 16;
    static const int rectWidthHealthBar = 16;
    static const int rectHeightHealthBar = 16;
    static const int rectWidthEnemy = 125;
    static const int rectHeightEnemy = 128;
    static const int rectWidthArrow = 16;
    static const int rectHeightArrow = 10;
    static const int rectWidthProjectileBar = 48;
    static const int rectHeightProjectileBar = 12;
    static const int rectWidthCoin = 25;
    static const int rectHeightCoin = 30;
    static const int coinOffsetX = 4;
    static const int rectWidthTree = 116;
    static const int rectHeightTree = 150;
    static const int rectWidthEagel = 36;
    static const int rectHeightEagel = 40;
    static const int eageloffsetX = 10;
    static const int eagelWidthOffset = 2;
    static const int rectWidthHairball = 500;
    static const int rectHeightHairball = 500;


private:

    std::map <TextureType, sf::Texture> textures;
    std::map<TextureType, sf::IntRect> textureCoordinates;
};