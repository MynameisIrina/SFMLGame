#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Camera.h"
#include "TextureLoader.h"
#include "ObstacleManager.h"
#include "Tile.h"

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


    Level_TileBased(const std::shared_ptr<Camera> cam, std::shared_ptr<TextureLoader> txLoaderRef);
    void Draw(const std::shared_ptr<sf::RenderWindow> window) const;
    void GenerateLevel(int startX);
    void PlacePattern(int patternIndex, int currentX, int currentY);
    void ShowGrid() const;
    void Initialize();
    void CheckGround(int curX, float v);
    void UpdateLevel(const std::shared_ptr<Player> player, float dt, bool respawn);
    void ShiftGridLeft();
    int FindRightmostTileX();
    std::vector<Tile>& GetAllTiles();
    int GenerateDefaultTiles();
    sf::Sprite CreateTile(TextureLoader::TextureType type, int coordX, int coordY, int x, int y, int globalPositionX);
    void CreateBoundRec(Tile::Tile_Type type, sf::Vector2f position, const std::shared_ptr<Obstacle> &obstacle = nullptr);

private:
    const int GRID_WIDTH = 25;
    const int GRID_HEIGHT = 15;
    const int TILE_SIZE = 32;
    const int BUFFER_COLUMNS = 10;
    const int TOTAL_GRID_WIDTH = GRID_WIDTH + BUFFER_COLUMNS;

    ObstacleManager obstacleManager;
    std::vector<Tile> tilesGround;
    std::vector<Tile> tilesObstacle;
    std::vector<Tile> allTiles;

    std::vector<std::vector<int>> grid;
    using Pattern = std::vector<std::vector<int>>;
    std::vector<Pattern> patterns;
    std::vector<sf::Sprite> tiles;

    std::shared_ptr<TextureLoader> txLoader;

    const std::shared_ptr<Player> player;
    const std::shared_ptr<Camera> camera;
    float previousPlayerX;
    float lastX_atTotalGridWidthPos = 0;
    float lastX_atGridWidthPos = 0;

    bool hasShifted = false;
    int shiftCounter = 0;
    int prevYFromLevelGen = 2;

    Pattern defaultPattern = {
        {0, 0, 0},
        {1, 1, 1},
        {1, 1, 1}};

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
        {0, 1},
        {1, 1}};

    Pattern pattern4 = {
        {0, 0},
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
        {0, 0},
        {0, 1},
        {1, 1}};

    Pattern pattern8 = {
        {0, 0, 0},
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


};