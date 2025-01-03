#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Camera.h"
#include "TextureLoader.h"

class Level_TileBased
{

public:
    enum LevelBounds
    {
        minX = 0,
        maxX = 25,
        minY = 2,
        maxY = 15
    };

    Level_TileBased(const std::shared_ptr<Player> pl, const std::shared_ptr<Camera> cam, std::shared_ptr<TextureLoader> txLoaderRef);
    void Draw(const std::shared_ptr<sf::RenderWindow> window) const;
    void GenerateLevel(int startX);
    void PlacePattern(int patternIndex, int currentX, int currentY);
    void ShowGrid() const;
    void Initialize();
    void CheckGround(int curX, float v);
    void UpdateLevel();
    void ShiftGridLeft();
    bool IsAreaFree(int currentX, int currentY, int patternWidth, int patternHeight);
    int FindRightmostTileX();
    std::vector<sf::RectangleShape> GetTiles();

private:
    const int GRID_WIDTH = 25;
    const int GRID_HEIGHT = 15;
    const int TILE_SIZE = 32;
    const int BUFFER_COLUMNS = 10;
    const int TOTAL_GRID_WIDTH = GRID_WIDTH + BUFFER_COLUMNS;

    std::vector<sf::RectangleShape> boundingRecs;

    std::vector<std::vector<int>> grid;
    using Pattern = std::vector<std::vector<int>>;
    std::vector<Pattern> patterns;
    std::vector<sf::Sprite> tiles;

    std::shared_ptr<TextureLoader> txLoader;
    sf::Sprite grassSprite;
    sf::Sprite dirtSprite;

    sf::Texture groundWithGrassTexture;
    sf::Sprite groundWithGrassSprite;

    const std::shared_ptr<Player> player;
    const std::shared_ptr<Camera> camera;
    float previousPlayerX;

    int startX = 2;
    bool hasShifted = false;
    int shiftCounter = 0;

    Pattern pattern1 = {
        {0, 0},
        {1, 1},
        {0, 1}};

    Pattern pattern2 = {
        {0, 0},
        {1, 0},
        {1, 1}};

    Pattern pattern3 = {
        {0, 1},
        {1, 1},
        {0, 1}};

    Pattern pattern4 = {
        {0, 0},
        {1, 1},
        {1, 1}};

    Pattern pattern5 = {
        {0, 1},
        {1, 1},
        {0, 0}};

    Pattern pattern6 = {
        {0, 0, 1},
        {0, 1, 1},
        {1, 1, 0}};

    Pattern pattern7 = {
        {1, 1},
        {0, 1},
        {0, 1}};

    Pattern pattern8 = {
        {0, 1},
        {1, 0},
        {0, 1}};

    Pattern pattern9 = {
        {1, 1, 1, 1},
        {1, 1, 1, 1},
        {1, 1, 1, 1}};

    Pattern pattern10 = {
        {1, 1},
        {1, 1},
        {1, 1},
        {1, 1}};

    Pattern pattern11 = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}};

    Pattern pattern12 = {
        {1, 0, 0},
        {1, 1, 1},
        {0, 0, 1}};
};