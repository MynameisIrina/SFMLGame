#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Camera.h"

class Level_TileBased
{

    enum TileCoordinates
    {
        groundX = 0,
        groundY = 10,
        grassX = 2,
        grassY = 8,
        tileSize = 32
    };

    enum LevelBounds
    {
        minX = 0,
        maxX = 25,
        minY = 2,
        maxY = 15
    };

public:
    Level_TileBased(const Player &pl, Camera &cam);
    void Draw(sf::RenderWindow &window) const;
    void GenerateLevel(int startX);
    void PlacePattern(int patternIndex, int currentX, int currentY);
    void ShowGrid() const;
    void Initialize();
    void CheckGround(int curX, float v);
    void UpdateLevel();
    void ShiftGridLeft();
    bool IsAreaFree(int currentX, int currentY, int patternWidth, int patternHeight);
    int FindRightmostTileX();

private:
    const int GRID_WIDTH = 25;
    const int GRID_HEIGHT = 15;
    const int TILE_SIZE = 32;
    const int BUFFER_COLUMNS = 10; 
    const int TOTAL_GRID_WIDTH = GRID_WIDTH + BUFFER_COLUMNS;


    std::vector<std::vector<int>> grid;
    std::vector<std::vector<std::vector<int>>> patterns;
    std::vector<sf::Sprite> tiles;

    sf::Texture groundWithGrassTexture;
    sf::Sprite groundWithGrassSprite;

    sf::Texture groundTexture;
    sf::Sprite groundSprite;

    const Player& player;
    const Camera& camera;
    float previousPlayerX;

    int startX = 2;
    bool hasShifted = false;
    int shiftCounter;

std::vector<std::vector<int>> pattern1 = {
    {0, 0},
    {1, 1},
    {0, 1}
};

std::vector<std::vector<int>> pattern2 = {
    {0, 0},
    {1, 0},
    {1, 1}
};

std::vector<std::vector<int>> pattern3 = {
    {0, 1},
    {1, 1},
    {0, 1}
};

std::vector<std::vector<int>> pattern4 = {
    {0, 0},
    {1, 1},
    {1, 1}
};

std::vector<std::vector<int>> pattern5 = {
    {0, 1},
    {1, 1},
    {0, 0}
};

std::vector<std::vector<int>> pattern6 = {
    {0, 0, 1},
    {0, 1, 1},
    {1, 1, 0}
};

std::vector<std::vector<int>> pattern7 = {
    {1, 1},
    {0, 1},
    {0, 1}
};

std::vector<std::vector<int>> pattern8 = {
    {0, 1},
    {1, 0},
    {0, 1}
};

std::vector<std::vector<int>> pattern9 = {
    {1, 1, 1, 1},
    {1, 1, 1, 1},
    {1, 1, 1, 1}
};

std::vector<std::vector<int>> pattern10 = {
    {1, 1},
    {1, 1},
    {1, 1},
    {1, 1}
};

std::vector<std::vector<int>> pattern11 = {
    {1, 1, 1},
    {1, 1, 1},
    {1, 1, 1}
};

std::vector<std::vector<int>> pattern12 = {
    {1, 0, 0},
    {1, 1, 1},
    {0, 0, 1}
};



};