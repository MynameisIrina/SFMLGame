#include "EnemyManager.h"
#include "Level_TileBased.h"

EnemyManager::EnemyManager(const std::shared_ptr<TextureLoader>& txLoader)
    : txLoader(txLoader)
{
    enemySprite = txLoader->SetSprite(TextureLoader::TextureType::Enemy);
}

void EnemyManager::SpawnEnemies(std::vector<std::vector<Tile>>& grid, int gridHeight, int gridWidth, int totalGridWidth, int lastX_atGridWidthPos, int tileSize)
{
    for (int y = 0; y < gridHeight - 1; ++y) {
        for (int x = gridWidth; x < totalGridWidth - 1; ++x) {
            if (CanPlaceEnemy(grid, x, y)) {
                PlaceEnemy(grid, gridWidth, lastX_atGridWidthPos, tileSize, x, y);
            }
        }
    }
}

bool EnemyManager::CanPlaceEnemy(const std::vector<std::vector<Tile>> &grid, int currX, int currY)
{
    bool noTileCurrent = grid[currY][currX].GetType() == 0;
    bool threeConsecutiveTilesUnderneath = ((grid[currY + 1][currX].GetType() == 2) && (grid[currY + 1][currX + 1].GetType() == 2) && (grid[currY + 1][currX - 1].GetType() == 2));
    bool noTilesAlongPath = (grid[currY][currX + 1].GetType() == 0 && grid[currY][currX - 1].GetType() == 0);

    if (noTileCurrent && threeConsecutiveTilesUnderneath && noTilesAlongPath)
    {
        return true;
    }
    return false;
}

void EnemyManager::PlaceEnemy(std::vector<std::vector<Tile>>& grid, int gridWidth, int lastX_atGridWidthPos, int tileSize, int currX, int currY)
{
    float globalTileX = lastX_atGridWidthPos + (currX - gridWidth) * tileSize;
    if (rand() % 4 == 0) {
        grid[currY][currX] = Tile(Tile::Enemy, sf::RectangleShape());
        ArrowPool pool(txLoader, 10);
        std::unique_ptr<Enemy> enemyArrow = std::make_unique<EnemyArrow>(std::move(pool));
        enemyArrow->Initialize(enemySprite, { globalTileX , static_cast<float>(currY * tileSize)}, 100, 10);
        enemies.push_back(std::move(enemyArrow));
    }
}

void EnemyManager::MoveEnemies(const std::shared_ptr<Player> player, const std::shared_ptr<Camera> camera, float dt)
{
    for (auto& enemy : enemies) {
        enemy->Update(player, camera, dt);
    }
}

void EnemyManager::Draw(const std::shared_ptr<sf::RenderWindow> window) const
{
    for (auto& enemy : enemies) {
        enemy->Draw(window);
    }
}