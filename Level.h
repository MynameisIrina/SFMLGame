#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Camera.h"
#include "TextureLoader.h"
#include "ObstacleManager.h"
#include "Tile.h"
#include "EnemyManager.h"
#include "CollectibleManager.h"

class Level
{

public:

    enum LevelBounds
    {
        minX = 0,
        maxX = 25,
        minY = 3,
        maxY = 15
    };

    Level(std::shared_ptr<ObstacleManager> obstacleManager, std::shared_ptr<EnemyManager> enemyManager, std::shared_ptr<CollectibleManager> collectibleManager, const std::shared_ptr<TextureLoader> &txLoader);
    void Draw(const std::shared_ptr<sf::RenderWindow> &window) const;
    void GenerateLevel(const int startX);
    void PlacePattern(const int patternIndex, const int height, const int width, const int currentX, const int currentY);
    void ShowGrid() const;
    void Initialize(const int firstPlatformHeight);
    void UpdateGround(const int startX, const float renderOffset);
    void UpdateLevel(const std::shared_ptr<Player> &player, const std::shared_ptr<Camera> &camera, const float dt);
    void ShiftGridLeft();
    int FindRightmostTileX();
    std::vector<sf::RectangleShape> &GetAllTiles();
    int GenerateDefaultTiles();
    sf::Sprite CreateSprite(TextureLoader::TextureType type, const int coordX, const int coordY, const int x, const int y, const int globalPositionX);
    TextureLoader::TextureType DetermineTextureType(int x, int y) const;
    Tile::Tile_Type DetermineTileType(int x, int y) const;
    void TrackLastGeneratedPosition(int worldPositionX, int x);
    void UpdateGrid(const Tile &tile, const int x, const int y);
    int AdjustPlatformHeight(const int previousHeight, const int currentHeight, const int maxGap, const int maxHeight, const int minHeight) const;
    void UpdateGenerationBounds(const sf::Vector2f& cameraCenter);
    void SpawnTree(const int startX, const float renderOffset, const int x, const int y);
    void Reset();

private:

    const int gridWidth = 25;
    const int gridHeight = 15;
    const int tileSize = 32;
    const int bufferColumns = 10;
    const int extraBuffer = 10;
    const int totalGridWidth = gridWidth + bufferColumns;
    int firstPlatformHeight = 5;
    int nextGridColumn = gridWidth + 1;
    const int treeGenerationProbability = 6;
    const int maxPlatformGap = 2;
    int bottomBuffer = 3;


    std::shared_ptr<ObstacleManager> obstacleManager;
    std::shared_ptr<EnemyManager> enemyManager;
    std::shared_ptr<CollectibleManager> collectibleManager;


    using Pattern = std::vector<std::vector<int>>;
    std::vector<std::vector<Tile>> grid;
    std::vector<Pattern> patterns;
    std::vector<sf::Sprite> tileSprites;
    std::vector<sf::Sprite> treeSprites;
    std::vector<Tile> groundTiles;
    std::vector<sf::RectangleShape> allTiles;

    std::shared_ptr<TextureLoader> txLoader;

    const std::shared_ptr<Player> player;
    float furthestTileX_totalGrid = 0;
    float furthestTileX_gridWidth = 0;

    bool hasShifted = false;
    int shiftCounter = 0;
    int prevYFromLevelGen = 8;

    Pattern defaultPattern = {
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1}};

    Pattern pattern1 = {
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1}};

    Pattern pattern2 = {
        {1, 1, 1},
        {1, 1, 1},
        {0, 1, 1}};

    Pattern pattern3 = {
        {1, 1, 1, 0, 0},
        {1, 1, 1, 0, 0}};

    Pattern pattern4 = {
        {1, 1, 1},
        {0, 1, 1},
        {1, 1, 0}};

    Pattern pattern5 = {
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1}};

    Pattern pattern6 = {
        {1, 0, 0, 1},
        {1, 0, 0, 1},
        {1, 1, 1, 1}};

    Pattern pattern7 = {
        {1, 1, 1, 1},
        {1, 0, 0, 1},
        {1, 0, 0, 1}};

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