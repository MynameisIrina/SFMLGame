#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Camera.h"
#include "TextureLoader.h"
#include "ObstacleManager.h"
#include "Tile.h"
#include "EnemyManager.h"

class Level
{

public:

    enum LevelBounds
    {
        minX = 0,
        maxX = 25,
        minY = 2,
        maxY = 15
    };


    Level(const std::shared_ptr<TextureLoader>& txLoaderRef);
    void Draw(const std::shared_ptr<sf::RenderWindow>& window) const;
    void GenerateLevel(const int startX);
    void PlacePattern(const int patternIndex, const int height, const int width, const int currentX, const int currentY);
    void ShowGrid() const;
    void Initialize(const int firstPlatformHeight);
    void UpdateGround(const int startX, const float renderOffset);
    void UpdateLevel(const std::shared_ptr<Player>& player, const std::shared_ptr<Camera>& camera, const float dt);
    void ShiftGridLeft();
    int FindRightmostTileX();
    std::vector<Tile>& GetAllTiles();
    int GenerateDefaultTiles();
    sf::Sprite CreateSprite(TextureLoader::TextureType type, const int coordX, const int coordY, const int x, const int y, const int globalPositionX);
    TextureLoader::TextureType DetermineTextureType(int x, int y) const;
    Tile::Tile_Type DetermineTileType(int x, int y) const;
    void TrackLastGeneratedPositions(int worldPositionX, int x);
    void UpdateGrid(const Tile& tile, const int x, const int y);
    int AdjustPlatformHeight(const int previousHeight, const int currentHeight, const int maxGap, const int maxHeight, const int minHeight) const;

private:
    const int gridWidth = 25;
    const int gridHeight = 15;
    const int tileSize = 32;
    const int bufferColumns = 10;
    const int totalGridWidth = gridWidth + bufferColumns;
    int firstPlatformHeight = 5;

    ObstacleManager obstacleManager;
    EnemyManager enemyManager;
    std::vector<Tile> allTiles;

    using Pattern = std::vector<std::vector<int>>;
    std::vector<std::vector<Tile>> grid;
    std::vector<Pattern> patterns;
    std::vector<sf::Sprite> tileSprites;
    std::vector<Tile> groundTiles;

    std::shared_ptr<TextureLoader> txLoader;

    const std::shared_ptr<Player> player;
    float previousPlayerX;
    float furthestTileX_totalGrid = 0;
    float furthestTileX_gridWidth = 0;

    bool hasShifted = false;
    int shiftCounter = 0;
    int prevYFromLevelGen = 8;

    Pattern defaultPattern = {
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1}};

    Pattern pattern1 = {
        {1, 1},
        {0, 1}};

    Pattern pattern2 = {
        {1, 0},
        {1, 1}};

    Pattern pattern3 = {
        {0, 1},
        {0, 1},
        {1, 1}};

    Pattern pattern4 = {
        {1, 1},
        {1, 1}};

    Pattern pattern5 = {
        {0, 1},
        {1, 1},
        {1, 0}};

    Pattern pattern6 = {
        {0, 0, 1},
        {0, 1, 1},
        {1, 1, 0}};

    Pattern pattern7 = {
        {0, 1},
        {1, 1}};

    Pattern pattern8 = {
        {1, 0, 0},
        {1, 1, 1}};

    Pattern pattern12 = {
        {0, 0, 1},
        {1, 1, 1},
        {1, 0, 1}};

    Pattern pattern13 = {
        {1},
        {1, 1},
        {1, 1, 1},
        {1, 1, 1, 1}};

    Pattern pattern14 = {
        {0, 0, 0, 1},
        {0, 0, 1, 1},
        {0, 1, 1, 1},
        {1, 1, 1, 1}};

    Pattern pattern15 = {
        {0, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1}};

    Pattern pattern16 = {
        {1, 1, 1, 0, 1, 1, 0},
        {1, 1, 1, 1, 1, 1, 1}};


};