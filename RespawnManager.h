#pragma once
#include <memory>
#include "Player.h"
#include "EnemyManager.h"
#include "CollectibleManager.h"

// class EnemyManager;
// class CollectibleManager;
// class Player;

class RespawnManager
{


public:
    RespawnManager(const std::shared_ptr<Player>& player,
                   const std::shared_ptr<EnemyManager>& enemyManager,
                   const std::shared_ptr<CollectibleManager>& collectibleManager);

    void RespawnAllEntities();

private:
    std::shared_ptr<Player> player;
    std::shared_ptr<EnemyManager> enemyManager;
    std::shared_ptr<CollectibleManager> collectibleManager;



};