#include "EnemyManager.h"
#include "Level.h"
#include "Eagle.h"
#include "ArrowPool.h"

EnemyManager::EnemyManager(const std::shared_ptr<TextureLoader> &txLoader, const std::shared_ptr<AudioManager> &audioManager)
    : txLoader(txLoader), audioManager(audioManager)
{
    enemySprite = txLoader->SetSprite(TextureLoader::TextureType::Enemy);
    eagleSprite = txLoader->SetSprite(TextureLoader::TextureType::Eagel);
}

void EnemyManager::SpawnEnemies(std::vector<std::vector<Tile>> &grid, const int maxY, const int minX, const int maxX, const int startX, const int tileSize)
{
    for (int y = 0; y < maxY - 1; ++y)
    {
        for (int x = minX; x < maxX - 1; ++x)
        {
            if (CanPlaceEnemy(grid, x, y, maxX, maxY))
            {
                PlaceEnemy(grid, tileSize, x, y);
            }
        }
    }
}

bool EnemyManager::CanPlaceEnemy(const std::vector<std::vector<Tile>> &grid, const int x, const int y, const int maxX, const int maxY)
{
    if (y - 1 < 0 || y + 1 >= maxY || x + 1 >= maxX || x - 1 < 0 || x + 2 >= maxX || x - 2 < 0)
        return false;
    
    if (rand() % 4 != 0)
        return false;

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

void EnemyManager::PlaceEnemy(std::vector<std::vector<Tile>> &grid, const int tileSize, const int x, const int y)
{
    const float globalTileX = grid[y+1][x].GetGlobalPosition().x + (tileSize * 0.5f);
    const float globalTileY = grid[y+1][x].GetGlobalPosition().y - 27.f;

    grid[y][x] = Tile(Tile::Enemy, sf::Vector2f(globalTileX, globalTileY), sf::RectangleShape());

    std::unique_ptr<Enemy> enemyArrow = std::make_unique<EnemyArrow>(std::make_unique<ArrowPool>(txLoader, 10), audioManager);
    sf::Vector2f position{globalTileX, globalTileY};
    enemyArrow->Initialize(enemySprite, position, 40, 10);
    enemies.push_back(std::move(enemyArrow));
}

void EnemyManager::UpdateEnemies(const std::shared_ptr<Player> &player, const std::shared_ptr<Camera> &camera, const std::shared_ptr<CollectibleManager> &collectibleManager, float dt)
{
    for (auto it = enemies.begin(); it != enemies.end();)
    {
        // Let the arrows fly when arrow enemy dies
        if (auto enemyArrow = dynamic_cast<EnemyArrow *>(((*it).get())))
        {
            enemyArrow->HandleFlyingArrows(player, camera, dt);
        }

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

    for (auto it = flyingEnemies.begin(); it != flyingEnemies.end();)
    {
        if ((*it)->GetState() == Enemy::State::Alive)
        {
            // erase enemies that are out of camera bounds
            if (auto eagle = dynamic_cast<Eagle *>((*it).get()))
            {
                sf::Vector2f position = eagle->GetPosition();
                bool withinBounds = eagle->GetDirection() == -1 ? position.x > camera->CalculateLeftBound() : position.x < camera->CalculateRightBound();
                if (!withinBounds)
                {
                    it = flyingEnemies.erase(it);
                    continue;
                }
            }

            (*it)->Update(player, camera, dt);
            ++it;
        }
        else if ((*it)->GetState() == Enemy::State::Dead)
        {
            (*it)->HandleDeath(collectibleManager);
            ++it;
        }
    }

    SpawnFlyingEnemy(player, camera, dt);
}

void EnemyManager::Draw(const std::shared_ptr<sf::RenderWindow> &window) const
{
    for (const auto &enemy : enemies)
    {
        enemy->Draw(window);
    }

    for (const auto &flyingEnemy : flyingEnemies)
    {
        flyingEnemy->Draw(window);
    }

}

std::vector<std::reference_wrapper<Enemy>> EnemyManager::GetAliveEnemies() const
{
    std::vector<std::reference_wrapper<Enemy>> aliveEnemies;

    for (const auto &enemy : enemies)
    {
        if (enemy->GetState() == Enemy::State::Alive)
        {
            aliveEnemies.push_back(*enemy);
        }
    }

    return aliveEnemies;
}

/*
    GetEnemiesBoundingBoxes() gets called frequently.
    Therefore cache a static object and return a reference
    to it instead of creating a new one every time.
*/

std::vector<sf::RectangleShape> &EnemyManager::GetEnemiesBoundingBoxes() const
{
    static std::vector<sf::RectangleShape> enemyBoundingBoxes;
    enemyBoundingBoxes.clear();
    enemyBoundingBoxes.reserve(enemies.size());

    for (const auto &enemy : enemies)
    {
        if (enemy->GetState() == Enemy::State::Alive)
        {
            enemyBoundingBoxes.emplace_back(enemy->GetBoundingBox());
        }
    }

    return enemyBoundingBoxes;
}

std::vector<sf::RectangleShape> &EnemyManager::GetFlyingEnemiesBoundingBoxes() const
{
    static std::vector<sf::RectangleShape> flyingEnemyBoundingBoxes;
    flyingEnemyBoundingBoxes.clear();
    flyingEnemyBoundingBoxes.reserve(flyingEnemies.size());

    for (const auto &flyingEnemy : flyingEnemies)
    {
        if (flyingEnemy->GetState() == Enemy::State::Alive)
        {
            flyingEnemyBoundingBoxes.emplace_back(flyingEnemy->GetBoundingBox());
        }
    }

    return flyingEnemyBoundingBoxes;
}

void EnemyManager::RespawnEnemies()
{
    for (auto &enemy : enemies)
    {
        enemy->Respawn();
    }
    
    flyingEnemySpawnTimer = 0.f;
}

void EnemyManager::SpawnFlyingEnemy(const std::shared_ptr<Player> &player, const std::shared_ptr<Camera> &camera, const float dt)
{
    const float spawnY = player->GetPosition().y;

    flyingEnemySpawnTimer += dt;

    if (flyingEnemySpawnTimer >= flyingEnemySpawnCooldown)
    {
        flyingEnemySpawnTimer = 0.f;

        const int spawnSide = rand() % 2;
        const float spawnX = spawnSide == 0 ? camera->CalculateLeftBound() : camera->CalculateRightBound();
        const float spawnY = player->GetPosition().y;

        sf::Vector2f spawnPosition{spawnX, spawnY};

        std::unique_ptr<Enemy> eagle = std::make_unique<Eagle>(audioManager);
        eagle->Initialize(eagleSprite, spawnPosition, 10, 5);

        if (auto *eaglePtr = dynamic_cast<Eagle *>(eagle.get()))
        {
            eaglePtr->SetMovementDirection(spawnSide == 0 ? 1 : -1);
        }

        flyingEnemies.push_back(std::move(eagle));
    }
}

void EnemyManager::Reset()
{
    enemies.clear();
    flyingEnemies.clear();
}
