#include "Level.h"
#include <iostream>

void Level::GenerateGround(sf::Sprite& ground)
{
    int gap = 31;
    for (int x = 0; x < 800; x += gap)
    {
        int y = 550;
        ground.setPosition(x, y);
        tiles.push_back(ground);
        bool makeGap = (static_cast<double>(std::rand()) / RAND_MAX > 0.85) ? true : false;
        std::cout<<static_cast<double>(std::rand()) / RAND_MAX<<std::endl;
        if(makeGap) gap += (40 + rand() % 30);
        else gap = 31;
    }
    GenerateSecondLevel(ground);
}

void Level::GenerateSecondLevel(sf::Sprite& ground)
{
    for(int x = 0; x < 800; x += 64)
    {
        int y = 400 - (rand() % 370);
        std::cout<< rand() % 400 <<std::endl;
        ground.setPosition(x, y);
        tiles.push_back(ground);
    }
}

std::vector<sf::Sprite> Level::Initialize()
{
    
    if (!groundTexture.loadFromFile("SFMLGame/Assets/Background/PixelArtPlatformer/Texture/Ground.png"))
    {
        std::cout << "Loading the ground failed." << std::endl;
    }
    else
    {
        sf::Sprite ground(groundTexture);
        int xIndex = 0;
        int yIndex = 8;
        ground.setTextureRect(sf::IntRect(xIndex * 32, yIndex * 32, 32, 64));
        GenerateGround(ground);
        return tiles;
    }
    return std::vector<sf::Sprite>();
}
