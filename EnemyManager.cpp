#include "EnemyManager.h"
#include "Level.h"

EnemyManager::EnemyManager(const std::shared_ptr<TextureLoader> &txLoader)
    : txLoader(txLoader)
{
    enemySprite = txLoader->SetSprite(TextureLoader::TextureType::Enemy);
}

void EnemyManager::SpawnEnemies(std::vector<std::vector<Tile>> &grid, const int maxY, const int minX, const int maxX, const int startX, const int tileSize)
{
    for (int y = 0; y < maxY - 1; ++y)
    {
        for (int x = minX; x < maxX - 1; ++x)
        {
            if (CanPlaceEnemy(grid, x, y))
            {
                PlaceEnemy(grid, minX, startX, tileSize, x, y);
            }
        }
    }
}

bool EnemyManager::CanPlaceEnemy(const std::vector<std::vector<Tile>> &grid, const int currX, const int currY)
{
    const auto &currentRow = grid[currY];
    const auto &bottomRow = grid[currY + 1];

    bool noTileCurrent = currentRow[currX].GetType() == 0;
    bool threeConsecutiveTilesUnderneath = ((bottomRow[currX].GetType() == 2) && (bottomRow[currX + 1].GetType() == 2) && (bottomRow[currX - 1].GetType() == 2));
    bool noTilesAlongPath = (currentRow[currX + 1].GetType() == 0 && currentRow[currX - 1].GetType() == 0);
    bool noEnemiesNear = (currentRow[currX + 1].GetType() != 4 && currentRow[currX - 1].GetType() != 4);

    if (noTileCurrent && threeConsecutiveTilesUnderneath && noTilesAlongPath && noEnemiesNear)
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

        std::unique_ptr<Enemy> enemyArrow = std::make_unique<EnemyArrow>((ArrowPool(txLoader, 10)));
        sf::Vector2f position{globalTileX, static_cast<float>(y * tileSize)};
        enemyArrow->Initialize(enemySprite, position, 50, 10);
        enemies.push_back(std::move(enemyArrow));
    }
}

void EnemyManager::UpdateEnemies(const std::shared_ptr<Player> &player, const std::shared_ptr<Camera> &camera, const std::shared_ptr<CollectibleManager>& collectibleManager, float dt)
{
    for (auto it = enemies.begin(); it != enemies.end();)
    {
        if ((*it)->GetState() == Enemy::State::Alive)
        {
            (*it)->Update(player, camera, dt);
            it++;
        }
        else if ((*it)->GetState() == Enemy::State::Dead)
        {
            (*it)->HandleDeath(collectibleManager);
            //it = enemies.erase(it);
            it++;
        }
    }
}

void EnemyManager::Draw(const std::shared_ptr<sf::RenderWindow> window) const
{
    for (const auto &enemy : enemies)
    {
        enemy->Draw(window);
    }
}

std::vector<std::reference_wrapper<Enemy>>& EnemyManager::GetAliveEnemies()
{
    static std::vector<std::reference_wrapper<Enemy>> aliveEnemies;
    for(const auto &enemy: enemies)
    {
        if(enemy->GetState() == Enemy::State::Alive)
        {
            aliveEnemies.push_back(*enemy);
        }
    }

    return aliveEnemies;
}

std::vector<sf::RectangleShape>& EnemyManager::GetEnemiesBoundingBoxes()
{
    static std::vector<sf::RectangleShape> enemyBoundingBoxes;

    enemyBoundingBoxes.clear();

    for(const auto &enemy: enemies)
    {
        if(enemy->GetState() == Enemy::State::Alive)
        {
            enemyBoundingBoxes.push_back(enemy->GetBoundingBox());
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
