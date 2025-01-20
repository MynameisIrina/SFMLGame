#include "ObstacleManager.h"
#include "Level_TileBased.h"

std::shared_ptr<TextureLoader> ObstacleManager::txLoader = nullptr; 

ObstacleManager::ObstacleManager(std::shared_ptr<TextureLoader> txLoaderRef)
{
    txLoader = txLoaderRef;
    sprite = txLoader->SetSprite(TextureLoader::TextureType::Obstacle);
}

void ObstacleManager::MoveObstacles(float dt)
{
    for (auto &obstacle : obstacles)
    {
        obstacle.first->MoveObstacle(dt);
        obstacle.first->UpdateTexture();
        obstacle.second.tileData.shape.setPosition(obstacle.first->GetPosition());
    }

}

void ObstacleManager::SpawnObstacles(std::vector<std::vector<int>> &grid, int gridHeight, int gridWidth, int totalGridWidth, int lastX_atGridWidthPos, int tileSize)
{
    for (int y = 0; y < gridHeight - 1; ++y)
    {
        for (int x = gridWidth; x < totalGridWidth - 1; ++x)
        {
            if (CanPlaceObstacle(grid, x, y))
            {
                PlaceObstacle(grid, gridWidth, lastX_atGridWidthPos, tileSize, x, y);
            }
        }
    }
}

bool ObstacleManager::CanPlaceObstacle(const std::vector<std::vector<int>> &grid, int currX, int currY)
{
    bool noTileCurrent = grid[currY][currX] == 0;
    bool threeConsecutiveTilesUnderneath = ((grid[currY + 1][currX] == 2) && (grid[currY + 1][currX + 1] == 2) && (grid[currY + 1][currX - 1] == 2));
    bool noTilesAlongPath = (grid[currY][currX + 1] == 0 && grid[currY][currX - 1] == 0);

    if (noTileCurrent && threeConsecutiveTilesUnderneath && noTilesAlongPath)
    {
        return true;
    }
    return false;
}

void ObstacleManager::PlaceObstacle(std::vector<std::vector<int>> &grid, int gridWidth, int lastX_atGridWidthPos, int tileSize, int currX, int currY)
{
    float globalTileX = lastX_atGridWidthPos + (currX - gridWidth) * tileSize;
    if (rand() % 2 == 0)
    {
        grid[currY][currX] = static_cast<int>(Tile::Tile_Type::Obstacle);
        std::shared_ptr<Obstacle> obstacle = std::make_shared<Obstacle>();
        obstacle->Initialize(sprite, { globalTileX, static_cast<float>(currY * tileSize)}, 50.0f, globalTileX - tileSize, globalTileX + tileSize);
        sf::RectangleShape boundRec = obstacle->CreateBoundingRec();
        obstacles.insert(std::make_pair(obstacle, Tile(Tile::Obstacle, boundRec)));
    }
}

void ObstacleManager::Draw(const std::shared_ptr<sf::RenderWindow> window) const
{
    for (auto &obstacle : obstacles)
    {
        window->draw(obstacle.first->GetSprite());
        window->draw(obstacle.second.tileData.shape);
    }
}

std::map<std::shared_ptr<Obstacle>, Tile> ObstacleManager::GetObstacles()
{
    return obstacles;
}
