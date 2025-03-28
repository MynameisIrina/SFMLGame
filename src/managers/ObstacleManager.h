#pragma once
#include "obstacles/Obstacle.h"
#include "level/Tile.h"

class Level_TileBased;

class ObstacleManager
{

private:
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    sf::Sprite sprite;
    std::shared_ptr<TextureLoader> txLoader;
    const int minSpeed = 60;
    const int maxSpeed = 80;
    const int obstacleProbability = 6;
    const int speedStep = 10;
    const float offsetY = 30.f;
    std::vector<sf::CircleShape> circles;

public:
    ObstacleManager(std::shared_ptr<TextureLoader> txLoaderRef);
    void UpdateObstacles(float dt);

    bool CanPlaceObstacle(const std::vector<std::vector<Tile>> &grid, const int x, const int y, const int maxX, const int maxY);
    void SpawnObstacles(std::vector<std::vector<Tile>> &grid, const int maxY, const int minX, const int maxX, const int tileSize);
    void PlaceObstacle(std::vector<std::vector<Tile>> &grid, const int tileSize, const int x, const int y);
    void Draw(const std::shared_ptr<sf::RenderWindow>& window) const;
    void Reset();

    std::vector<sf::RectangleShape>& GetObstaclesBoundingBoxes() const;
};