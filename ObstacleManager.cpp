#include "ObstacleManager.h"
#include "Level_TileBased.h"


ObstacleManager::ObstacleManager(std::shared_ptr<TextureLoader> txLoaderRef): txLoader(txLoaderRef)
{
    sprite = txLoader->SetSprite(TextureLoader::TextureType::Obstacle);
}

void ObstacleManager::MoveObstacles(float dt)
{
    for (auto &obstacle : obstacles)
    {
        obstacle.MoveObstacle(dt);
        obstacle.UpdateTexture();
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

    if (rand() % OBSTACLE_PROBABILITY == 0)
    {
        grid[y][x] = Tile(Tile::Tile_Type::Obstacle, sf::RectangleShape());
        Obstacle obstacle;
        float speed = std::clamp(rand() % MAX_SPEED, MIN_SPEED, MAX_SPEED);
        obstacle.Initialize(sprite, {globalTileX + sprite.getGlobalBounds().width / 2, static_cast<float>(y * tileSize + sprite.getGlobalBounds().height / 2)}, speed, globalTileX - tileSize + sprite.getGlobalBounds().width / 2, globalTileX + tileSize + sprite.getGlobalBounds().width / 2);
        obstacles.push_back(obstacle);
    }
}

void ObstacleManager::Draw(const std::shared_ptr<sf::RenderWindow> window) const
{
    for (auto &obstacle : obstacles)
    {
        window->draw(obstacle.GetSprite());
        window->draw(obstacle.GetBoundingBox());
    }
}

std::vector<Obstacle> ObstacleManager::GetObstacles() const
{
    return obstacles;
}
