#include "EnemyManager.h"
#include "Level.h"

EnemyManager::EnemyManager(const std::shared_ptr<TextureLoader> &txLoader, const std::shared_ptr<AudioManager> &audioManager)
    : txLoader(txLoader), audioManager(audioManager)
{
    enemySprite = txLoader->SetSprite(TextureLoader::TextureType::Enemy);
}

void EnemyManager::SpawnEnemies(std::vector<std::vector<Tile>> &grid, const int maxY, const int minX, const int maxX, const int startX, const int tileSize)
{
    for (int y = 0; y < maxY - 1; ++y)
    {
        for (int x = minX; x < maxX - 1; ++x)
        {
            if (CanPlaceEnemy(grid, x, y, maxX, maxY))
            {
                PlaceEnemy(grid, minX, startX, tileSize, x, y);
            }
        }
    }
}

bool EnemyManager::CanPlaceEnemy(const std::vector<std::vector<Tile>> &grid, const int x, const int y, const int maxX, const int maxY)
{
    if (y - 1 < 0 || y + 1 >= maxY || x + 1 >= maxX || x - 1 < 0 || x + 2 >= maxX || x - 2 < 0)
    {
        return false;
    }

    const auto &currentRow = grid[y];
    const auto &bottomRow = grid[y + 1];

    bool noTileCurrent = currentRow[x].GetType() == 0;
    bool threeConsecutiveTilesUnderneath = ((bottomRow[x].GetType() == 2) && (bottomRow[x + 1].GetType() == 2) && (bottomRow[x - 1].GetType() == 2));
    bool noTilesAlongPath = (currentRow[x + 1].GetType() == 0 && currentRow[x - 1].GetType() == 0);
    bool noEnemiesNear = (currentRow[x + 1].GetType() != 4 && currentRow[x - 1].GetType() != 4) && (currentRow[x + 2].GetType() != 4 && currentRow[x - 2].GetType() != 4);
    bool noObstaclesNear = (currentRow[x + 1].GetType() != 3 && currentRow[x - 1].GetType() != 3) && (currentRow[x + 2].GetType() != 3 && currentRow[x - 2].GetType() != 3);

    if (noTileCurrent && threeConsecutiveTilesUnderneath && noTilesAlongPath && noEnemiesNear && noObstaclesNear)
    {
        return true;
    }

    return false;
}

void EnemyManager::PlaceEnemy(std::vector<std::vector<Tile>> &grid, const int minX, const int startX, const int tileSize, const int x, const int y)
{
    float globalTileX = startX + static_cast<float>((x - minX) * tileSize) + (tileSize * 0.5f);

    if (rand() % 8 == 0)
    {
        grid[y][x] = Tile(Tile::Enemy, sf::RectangleShape());

        std::unique_ptr<Enemy> enemyArrow = std::make_unique<EnemyArrow>((ArrowPool(txLoader, 10)), audioManager);
        sf::Vector2f position{globalTileX, static_cast<float>(y * tileSize)};
        enemyArrow->Initialize(enemySprite, position, 50, 10);
        enemies.push_back(std::move(enemyArrow));
    }
}

void EnemyManager::UpdateEnemies(const std::shared_ptr<Player> &player, const std::shared_ptr<Camera> &camera, const std::shared_ptr<CollectibleManager> &collectibleManager, float dt)
{
    for (auto it = enemies.begin(); it != enemies.end();)
    {
        if ((*it)->GetState() == Enemy::State::Alive)
        {
            (*it)->Update(player, camera, dt);
            ++it;
        }
        else if ((*it)->GetState() == Enemy::State::Dead)
        {
            (*it)->HandleDeath(collectibleManager);
            ++it;
        }
    }
}

void EnemyManager::Draw(const std::shared_ptr<sf::RenderWindow> &window) const
{
    for (const auto &enemy : enemies)
    {
        enemy->Draw(window);
    }
}

std::vector<std::reference_wrapper<Enemy>> &EnemyManager::GetAliveEnemies()
{
    static std::vector<std::reference_wrapper<Enemy>> aliveEnemies;
    for (const auto &enemy : enemies)
    {
        if (enemy->GetState() == Enemy::State::Alive)
        {
            aliveEnemies.push_back(*enemy);
        }
    }

    return aliveEnemies;
}

std::vector<sf::RectangleShape> &EnemyManager::GetEnemiesBoundingBoxes()
{
    static std::vector<sf::RectangleShape> enemyBoundingBoxes;

    enemyBoundingBoxes.clear();

    for (const auto &enemy : enemies)
    {
        if (enemy->GetState() == Enemy::State::Alive)
        {
            enemyBoundingBoxes.emplace_back(enemy->GetBoundingBox());
        }
    }

    return enemyBoundingBoxes;
}

void EnemyManager::RespawnEnemies()
{
    for (auto &enemy : enemies)
    {
        enemy->Respawn();
    }
}
