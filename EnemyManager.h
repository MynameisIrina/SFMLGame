#pragma once
#include <memory>
#include <vector>
#include "Enemy.h"
#include "EnemyArrow.h"
#include "TextureLoader.h"

class EnemyManager {
public:

    EnemyManager(const std::shared_ptr<TextureLoader>& txLoader, const std::shared_ptr<AudioManager> &audioManager);
    void SpawnEnemies(std::vector<std::vector<Tile>> &grid, const int maxY, const int minX, const int maxX, const int startX, const int tileSize);
    void UpdateEnemies(const std::shared_ptr<Player>& player,const std::shared_ptr<Camera>& camera, const std::shared_ptr<CollectibleManager>& collectibleManager, const float dt);
    void Draw(const std::shared_ptr<sf::RenderWindow>& window) const;
    bool CanPlaceEnemy(const std::vector<std::vector<Tile>> &grid, const int x, const int y, const int maxX, const int maxY);
     void PlaceEnemy(std::vector<std::vector<Tile>> &grid,const int minX,const int startX,const int tileSize,const int x,const int y);
    std::vector<std::reference_wrapper<Enemy>>& GetAliveEnemies();
    std::vector<sf::RectangleShape>& GetEnemiesBoundingBoxes();
    void RespawnEnemies();

private:
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::shared_ptr<TextureLoader> txLoader;
    std::shared_ptr<AudioManager> audioManager;


    sf::Sprite enemySprite;
    const float offsetY = 12;

};