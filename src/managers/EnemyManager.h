#pragma once
#include "enemies/Enemy.h"
#include "enemies/enemyarrow/EnemyArrow.h"
#include "graphics/TextureLoader.h"
#include "level/Tile.h"

class EnemyManager {
public:

    EnemyManager(const std::shared_ptr<TextureLoader>& txLoader, const std::shared_ptr<AudioManager> &audioManager);
    void SpawnEnemies(std::vector<std::vector<Tile>> &grid, const int maxY, const int minX, const int maxX, const int startX, const int tileSize);
    void UpdateEnemies(const std::shared_ptr<Player>& player,const std::shared_ptr<Camera>& camera, const std::shared_ptr<CollectibleManager>& collectibleManager, const float dt);
    void Draw(const std::shared_ptr<sf::RenderWindow>& window) const;
    bool CanPlaceEnemy(const std::vector<std::vector<Tile>> &grid, const int x, const int y, const int maxX, const int maxY);
     void PlaceEnemy(std::vector<std::vector<Tile>> &grid, const int tileSize, const int x, const int y);
    std::vector<std::reference_wrapper<Enemy>> GetAliveEnemies() const;
    std::vector<sf::RectangleShape>& GetEnemiesBoundingBoxes() const;
    std::vector<sf::RectangleShape>& GetFlyingEnemiesBoundingBoxes() const;
    void RespawnEnemies();
    void SpawnFlyingEnemy(const std::shared_ptr<Player>& player, const std::shared_ptr<Camera> &camera, const float dt);
    void Reset();

private:
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Enemy>> flyingEnemies;
    std::shared_ptr<TextureLoader> txLoader;
    std::shared_ptr<AudioManager> audioManager;

    sf::Sprite enemySprite;
    sf::Sprite eagleSprite;
    const float offsetY = 27.f;
    float flyingEnemySpawnTimer = 0.0f;
    float flyingEnemySpawnCooldown = 10.0f; 
    const int spawnEnemyProbability = 4;

    const int arrowEnemyHealth = 40;
    const int arrowEnemyDamage = 10;
    const int arrowEnemyArrowPool = 10;

    const int eagleEnemyHealth = 10;
    const int eagleEnemyDamage = 5;

};