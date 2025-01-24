#pragma once
#include <memory>
#include <vector>
#include "Enemy.h"
#include "EnemyArrow.h"
#include "TextureLoader.h"

class EnemyManager {
public:
    EnemyManager(const std::shared_ptr<TextureLoader>& txLoader);
    void SpawnEnemies(std::vector<std::vector<Tile>>& grid, int gridHeight, int gridWidth, int totalGridWidth, int lastX_atGridWidthPos, int tileSize);
    void MoveEnemies(const std::shared_ptr<Player> player, float dt);
    void Draw(const std::shared_ptr<sf::RenderWindow> window) const;
    bool CanPlaceEnemy(const std::vector<std::vector<Tile>> &grid, int currX, int currY);

private:
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::shared_ptr<TextureLoader> txLoader;
    sf::Sprite enemySprite;

    void PlaceEnemy(std::vector<std::vector<Tile>>& grid, int gridWidth, int lastX_atGridWidthPos, int tileSize, int currX, int currY);
};