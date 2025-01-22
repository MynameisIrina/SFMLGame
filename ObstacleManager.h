#pragma once
#include <map>
#include "Obstacle.h"
#include "Tile.h"

class Level_TileBased;

class ObstacleManager
{

private:
    std::vector<std::shared_ptr<Obstacle>> obstacles;
    sf::Sprite sprite;
    static std::shared_ptr<TextureLoader> txLoader;
    const int MIN_SPEED = 45;
    const int MAX_SPEED = 60;
    const int OBSTACLE_PROBABILITY = 8;
public:
    ObstacleManager(std::shared_ptr<TextureLoader> txLoaderRef);
    void MoveObstacles(float dt);
    void SpawnObstacles(std::vector<std::vector<Tile>> &grid,int startX, int startY, int totalGridWidth, int x, int tileSize);
    bool CanPlaceObstacle(const std::vector<std::vector<Tile>> &grid, int x, int y,  int maxX, int maxY);
    void PlaceObstacle(std::vector<std::vector<Tile>> &grid,  int gridWidth, int startX, int tileSize, int x, int y);
    void Draw(const std::shared_ptr<sf::RenderWindow> window) const;
    std::vector<std::shared_ptr<Obstacle>> GetObstacles();
};