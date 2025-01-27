#include "EnemyManager.h"
#include "Level.h"

EnemyManager::EnemyManager(const std::shared_ptr<TextureLoader>& txLoader)
    : txLoader(txLoader)
{
    enemySprite = txLoader->SetSprite(TextureLoader::TextureType::Enemy);
}

void EnemyManager::SpawnEnemies(std::vector<std::vector<Tile>> &grid, int maxY, int minX, int maxX, int startX, int tileSize)
{
    for (int y = 0; y < maxY - 1; ++y) {
        for (int x = minX; x < maxX - 1; ++x) {
            if (CanPlaceEnemy(grid, x, y)) {
                PlaceEnemy(grid, minX, startX, tileSize, x, y);
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

void EnemyManager::PlaceEnemy(std::vector<std::vector<Tile>> &grid, int minX, int startX, int tileSize, int x, int y)
{
    float globalTileX = startX + (x - minX) * tileSize;
    if (rand() % 4 == 0) 
    {
        grid[y][x] = Tile(Tile::Enemy, sf::RectangleShape());
        ArrowPool pool(txLoader, 10);
        std::unique_ptr<Enemy> enemyArrow = std::make_unique<EnemyArrow>(std::move(pool));
        sf::Vector2f position{globalTileX , static_cast<float>(y * tileSize) - offsetY };
        enemyArrow->Initialize(enemySprite, position, 100, 10);
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


std::vector<std::unique_ptr<Enemy>>& EnemyManager::GetEnemies()
{
    return enemies;
}
