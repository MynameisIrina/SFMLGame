#include "managers/ObstacleManager.h"
#include "level/Level.h"

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

void ObstacleManager::SpawnObstacles(std::vector<std::vector<Tile>> &grid, const int maxY, const int minX, const int maxX, const int tileSize)
{
    for (int y = 0; y < maxY; ++y)
    {
        for (int x = minX; x < maxX; ++x)
        {
            if (CanPlaceObstacle(grid, x, y, maxX, maxY))
            {
                PlaceObstacle(grid, tileSize, x, y);
            }
        }
    }
}

bool ObstacleManager::CanPlaceObstacle(const std::vector<std::vector<Tile>> &grid, int x, int y, int maxX, int maxY)
{
    if (y - 1 < 0 || y + 1 >= maxY || x + 1 >= maxX || x - 1 < 0 || x + 2 >= maxX || x - 2 < 0)
        return false;

    if (rand() % obstacleProbability != 0)
        return false;

    const auto &currentRow = grid[y];
    const auto &bottomRow = grid[y + 1];

    const bool noTileCurrent = currentRow[x].GetType() == Tile::Empty;
    const bool threeConsecutiveTilesUnderneath = ((bottomRow[x].GetType() == Tile::Grass) && (bottomRow[x + 1].GetType() == Tile::Grass) && (bottomRow[x - 1].GetType() == Tile::Grass));
    const bool noTilesAlongPath = (currentRow[x + 1].GetType() == Tile::Empty && currentRow[x - 1].GetType() == Tile::Empty);
    const bool noObstaclesNear = (currentRow[x + 1].GetType() != Tile::Obstacle && currentRow[x - 1].GetType() != Tile::Obstacle) && (currentRow[x + 2].GetType() != Tile::Obstacle && currentRow[x - 2].GetType() != Tile::Obstacle);
    const bool noEnemiesNear = (currentRow[x + 1].GetType() != Tile::Enemy && currentRow[x - 1].GetType() != Tile::Enemy);

    if (noTileCurrent && threeConsecutiveTilesUnderneath && noTilesAlongPath && noObstaclesNear)
    {
        return true;
    }

    return false;
}

void ObstacleManager::PlaceObstacle(std::vector<std::vector<Tile>> &grid, const int tileSize, const int x, const int y)
{
    // spawn obstacle right above the platform
    const float globalTileX = grid[y + 1][x].GetGlobalPosition().x + (tileSize * 0.5f);
    const float globalTileY = grid[y + 1][x].GetGlobalPosition().y - offsetY;

    const float minPosX = globalTileX - tileSize;
    const float maxPosX = globalTileX + tileSize;

    grid[y][x] = Tile(Tile::Tile_Type::Obstacle, sf::Vector2f(globalTileX, globalTileY), sf::RectangleShape());
    std::unique_ptr<Obstacle> obstacle = std::make_unique<Obstacle>();
    
    const int range = (maxSpeed - minSpeed) / speedStep + 1;
    const int randomSpeed = minSpeed + (std::rand() % range) * speedStep;
    obstacle->Initialize(sprite, sf::Vector2f(globalTileX, globalTileY), randomSpeed, minPosX, maxPosX);
    obstacles.push_back(std::move(obstacle));
}

void ObstacleManager::Draw(const std::shared_ptr<sf::RenderWindow> &window) const
{
    for (auto &obstacle : obstacles)
    {
        obstacle->Draw(window);
    }
}

void ObstacleManager::Reset()
{
    obstacles.clear();
}

std::vector<sf::RectangleShape> &ObstacleManager::GetObstaclesBoundingBoxes() const
{
    static std::vector<sf::RectangleShape> boxes;

    boxes.clear();

    for (const auto &obstacle : obstacles)
    {
        boxes.emplace_back(obstacle->GetBoundingBox());
    }

    return boxes;
}
