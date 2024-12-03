#include "Level.h"
#include <iostream>

Level::Level(const Player &pl) : player(pl), previousPlayerX(pl.GetPosition().x) {}

void Level::GenerateGround(sf::Sprite &ground, sf::Sprite &obstacle )
{
     int gap = 32;
    for (int x = positionOfLastTileGround.x; x <= positionOfLastTileGround.x + 800; x+=gap)
    {
        int y = 550;

        // decide whether the obstacle will be added
        bool insertObstacle = (static_cast<double>(std::rand()) / RAND_MAX > 0.85) ? true : false;
        if(insertObstacle)
        {
            obstacle.setPosition(x,y + 17.f);
            tilesGround.push_back(obstacle);
        }
        else
        {
            ground.setPosition(x, y);
            tilesGround.push_back(ground);
        }
    }
    positionOfLastTileGround = tilesGround.back().getPosition();
    
}

// generate tiles above the ground
void Level::GenerateSecondLevel(sf::Sprite &ground)
{
    for (int x = positionOfLastTileUp.x; x < positionOfLastTileUp.x + 800; x += 64)
    {
        int y = 400 - (rand() % 370);
        ground.setPosition(x, y);
        tilesUp.push_back(ground);
    }
    positionOfLastTileUp = tilesUp.back().getPosition();
}

void Level::Update()
{
    float deltaX = player.GetPosition().x - previousPlayerX;
    previousPlayerX = player.GetPosition().x;

    UpdateGround(deltaX);
    UpdateUp(deltaX);

}

void Level::UpdateUp(float deltaX)
{

    if(deltaX > 0.f)
    {
        for(sf::Sprite &tile: tilesUp)
        {
            tile.move(-0.05f, 0.f);
        }

        positionOfLastTileUp = tilesUp.back().getPosition();
    }

    if(positionOfLastTileUp.x <= 800.f)
    {
        GenerateSecondLevel(groundSprite);
    }

}

void Level::UpdateGround(float deltaX)
{

    if(deltaX > 0.f)
    {
        for(sf::Sprite &tile: tilesGround)
        {
            tile.move(-0.05f, 0.f);
        }

        positionOfLastTileGround = tilesGround.back().getPosition();

    }

    if(positionOfLastTileGround.x <= 800.f)
    {
        GenerateGround(groundSprite, obstacleSprite);
    }
}

std::vector<sf::Sprite>& Level::InitializeGround()
{

    if (!groundTexture.loadFromFile("SFMLGame/Assets/Background/PixelArtPlatformer/Texture/Ground.png") ||
        !obstacleTexture.loadFromFile("SFMLGame/Assets/Background/PixelArtPlatformer/Texture/VillageDetails.png"))
    {
        std::cout << "Loading the ground failed." << std::endl;
        static std::vector<sf::Sprite> emptyTiles;
        return emptyTiles;
    }
    else
    {
        groundSprite.setTexture(groundTexture);
        int xIndex = 0;
        int yIndex = 8;
        groundSprite.setTextureRect(sf::IntRect(xIndex * 32, yIndex * 32, 32, 64));

        obstacleSprite.setTexture(obstacleTexture);
        int x = 25;
        int y = 3;
        obstacleSprite.setTextureRect(sf::IntRect(x * 32, y * 32, 32, 32));

        positionOfLastTileGround = sf::Vector2f(0.f,0.f);
        GenerateGround(groundSprite, obstacleSprite);
        length = 800;
        return tilesGround;
    }
}

std::vector<sf::Sprite>& Level::InitializeUp()
{
    if (!groundTexture.loadFromFile("SFMLGame/Assets/Background/PixelArtPlatformer/Texture/Ground.png"))
    {
        std::cout << "Loading the ground failed." << std::endl;
        static std::vector<sf::Sprite> emptyTiles;
        return emptyTiles;
    }
    else
    {
        groundSprite.setTexture(groundTexture);
        int xIndex = 0;
        int yIndex = 8;
        groundSprite.setTextureRect(sf::IntRect(xIndex * 32, yIndex * 32, 32, 64));
        positionOfLastTileUp = sf::Vector2f(0.f,0.f);
        GenerateSecondLevel(groundSprite);
        length = 800;
        return tilesUp;
    }
    
}
