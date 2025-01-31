#include "ObstacleManager.h"
#include "Level.h"

ObstacleManager::ObstacleManager(std::shared_ptr<TextureLoader> txLoaderRef) : txLoader(txLoaderRef)
{
    sprite = txLoader->SetSprite(TextureLoader::TextureType::Obstacle);
}

void ObstacleManager::UpdateObstacles(float dt)
{
    for (auto &obstacle : obstacles)
    {
        obstacle->MoveObstacle(dt);
    }
}

void ObstacleManager::SpawnObstacles(std::vector<std::vector<Tile>> &grid, const int maxY, const int minX, const int maxX, const int startX, const int tileSize)
{
    for (int y = 0; y < maxY; ++y)
    {
        for (int x = minX; x < maxX; ++x)
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
    if (y - 1 < 0 || y + 1 >= maxY || x + 1 >= maxX || x - 1 < 0)
    {
        return false;
    }

    const auto &currentRow = grid[y];
    const auto &bottomRow = grid[y + 1];

    bool noTileCurrent = currentRow[x].GetType() == Tile::Empty;
    bool threeConsecutiveTilesUnderneath = ((bottomRow[x].GetType() == Tile::Grass) && (bottomRow[x + 1].GetType() == Tile::Grass) && (bottomRow[x - 1].GetType() == Tile::Grass));
    bool noTilesAlongPath = (currentRow[x + 1].GetType() == Tile::Empty && currentRow[x - 1].GetType() == Tile::Empty);
    bool noObstaaclesNear = (currentRow[x + 1].GetType() != Tile::Obstacle && currentRow[x - 1].GetType() != Tile::Obstacle);

    if (noTileCurrent && threeConsecutiveTilesUnderneath && noTilesAlongPath && noObstaaclesNear)
    {
        return true;
    }

    return false;
}

void ObstacleManager::PlaceObstacle(std::vector<std::vector<Tile>> &grid, const int minX, const int startX, const int tileSize, const int x, const int y)
{
    if (rand() % obstacleProbability != 0)
        return;

    const float globalTileX = startX + (x - minX) * tileSize + (tileSize * 0.5f);
    const float globalTileY = static_cast<float>(y * tileSize);

    grid[y][x] = Tile(Tile::Tile_Type::Obstacle, sf::RectangleShape());

    std::unique_ptr<Obstacle> obstacle = std::make_unique<Obstacle>();
    const float speed = std::clamp(rand() % maxSpeed, minSpeed, maxSpeed);
    float minPosX = globalTileX - tileSize;
    float maxPosX = globalTileX + tileSize;

    obstacle->Initialize(sprite, sf::Vector2f(globalTileX, globalTileY), speed, minPosX, maxPosX);
    const float offsetY = obstacle->GetBoundingBox().getGlobalBounds().height / 2;
    obstacle->CreateVisualLine(minPosX, maxPosX, globalTileY + offsetY, globalTileY + offsetY);

    obstacles.push_back(std::move(obstacle));
}

void ObstacleManager::Draw(const std::shared_ptr<sf::RenderWindow> &window) const
{
    for (auto &obstacle : obstacles)
    {
        obstacle->Draw(window);
    }
}

std::vector<std::unique_ptr<Obstacle>> &ObstacleManager::GetObstacles()
{
    return obstacles;
}
