#pragma once
#include <map>
#include "Obstacle.h"
#include "Tile.h"

class Level_TileBased;

class ObstacleManager
{

private:

    std::map<std::shared_ptr<Obstacle>, Tile> obstacles;
    sf::Sprite sprite;
    static std::shared_ptr<TextureLoader> txLoader;

public:
    ObstacleManager(std::shared_ptr<TextureLoader> txLoaderRef);
    void MoveObstacles(float dt);
    void SpawnObstacles(std::vector<std::vector<int>> &grid, int gridHeight, int gridWidth, int totalGridWidth, int lastX_atGridWidthPos, int tileSize);
    bool CanPlaceObstacle(const std::vector<std::vector<int>> &grid, int currX, int currY);
    void PlaceObstacle(std::vector<std::vector<int>> &grid, int gridWidth, int lastX_atGridWidthPos, int tileSize, int currX, int currY);
    void Draw(const std::shared_ptr<sf::RenderWindow> window) const;
    std::map<std::shared_ptr<Obstacle>, Tile> GetObstacles();

};