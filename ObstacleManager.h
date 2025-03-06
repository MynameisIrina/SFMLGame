#pragma once
#include "Obstacle.h"
#include "Tile.h"

class Level_TileBased;

class ObstacleManager
{

private:
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    sf::Sprite sprite;
    std::shared_ptr<TextureLoader> txLoader;
    const int minSpeed = 45;
    const int maxSpeed = 60;
    const int speed = 50;
    const int obstacleProbability = 10;
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