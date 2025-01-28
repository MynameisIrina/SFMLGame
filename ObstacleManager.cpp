#include "ObstacleManager.h"
#include "Level.h"


ObstacleManager::ObstacleManager(std::shared_ptr<TextureLoader> txLoaderRef): txLoader(txLoaderRef)
{
    sprite = txLoader->SetSprite(TextureLoader::TextureType::Obstacle);
}

void ObstacleManager::UpdateObstacles(float dt)
{
    for (auto &obstacle : obstacles)
    {
        obstacle->MoveObstacle(dt);
        obstacle->UpdateTexture();
    }
}

void ObstacleManager::SpawnObstacles(std::vector<std::vector<Tile>> &grid, int maxY, int minX, int maxX, int startX, int tileSize)
{
    for (int y = 0; y < maxY; ++y)
    {
        for (int x = minX; x < maxX ; ++x)
        {
            if (CanPlaceObstacle(grid, x, y, maxX, maxY))
            {
                PlaceObstacle(grid, minX, startX, tileSize, x, y);
            }
        }
    }
}

bool ObstacleManager::CanPlaceObstacle(const std::vector<std::vector<Tile>> &grid, int x, int y, int maxX, int maxY)
{
    if(y - 1 < 0 || y + 1 >= maxY || x + 1 >= maxX || x - 1 < 0 )
    {
        return false;
    }

    bool noTileCurrent = grid[y][x].GetType() == Tile::Empty;
    bool threeConsecutiveTilesUnderneath = ((grid[y + 1][x].GetType() == Tile::Grass) && (grid[y + 1][x + 1].GetType() == Tile::Grass) && (grid[y + 1][x - 1].GetType() == Tile::Grass));
    bool noTilesAlongPath = (grid[y][x + 1].GetType() == Tile::Empty && grid[y][x- 1].GetType() == Tile::Empty);

    if (noTileCurrent && threeConsecutiveTilesUnderneath && noTilesAlongPath)
    {
        return true;
    }

    return false;
}

void ObstacleManager::PlaceObstacle(std::vector<std::vector<Tile>> &grid, int minX, int startX, int tileSize, int x, int y)
{
    float globalTileX = startX + (x - minX) * tileSize;

    if (rand() % obstacleProbability == 0)
    {
        grid[y][x] = Tile(Tile::Tile_Type::Obstacle, sf::RectangleShape());
        std::unique_ptr<Obstacle> obstacle = std::make_unique<Obstacle>();
        float speed = std::clamp(rand() % maxSpeed, minSpeed, maxSpeed);
        sf::Vector2f obstaclePosition{globalTileX, static_cast<float>(y * tileSize)};
        float minX = globalTileX - tileSize;
        float maxX = globalTileX + tileSize;
        obstacle->Initialize(sprite, obstaclePosition, speed, minX, maxX);
        obstacle->CreateVisualLine(minX, maxX , obstaclePosition.y, obstaclePosition.y);
        obstacles.push_back(std::move(obstacle));
    }
}

void ObstacleManager::Draw(const std::shared_ptr<sf::RenderWindow> window) const
{
    for (auto &obstacle : obstacles)
    {
        obstacle->Draw(window);
    }
}

std::vector<std::unique_ptr<Obstacle>>& ObstacleManager::GetObstacles()
{
    return obstacles;
}
