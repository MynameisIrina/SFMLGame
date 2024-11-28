#pragma once
#include <SFML/Graphics.hpp>

class Level
{

public:
    void GenerateGround(sf::Sprite& ground);
    std::vector<sf::Sprite> Initialize();
    void GenerateSecondLevel(sf::Sprite& ground);

private:
    sf::Texture groundTexture;
    std::vector<sf::Sprite> tiles;


};