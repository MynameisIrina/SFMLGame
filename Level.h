#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"

class Level
{

public:
    Level(const Player& pl);
    void GenerateGround(sf::Sprite& ground, sf::Sprite &obstacle);
    std::vector<sf::Sprite>& InitializeGround();
    std::vector<sf::Sprite>& InitializeUp();
    void GenerateSecondLevel(sf::Sprite& ground);
    //void UpdateLevel(float dt);
    void UpdateGround(float dt);
    void UpdateUp(float dt);
    void Update();

private:
    //view
    const Player& player;
    float previousPlayerX;
    sf::Texture groundTexture;
    sf::Sprite groundSprite;
    sf::Texture obstacleTexture;
    sf::Sprite obstacleSprite;
    //model
    std::vector<sf::Sprite> tilesGround;
    std::vector<sf::Sprite> tilesUp;
    sf::Vector2f positionOfLastTileGround;
    sf::Vector2f positionOfLastTileUp;
    int length;
};