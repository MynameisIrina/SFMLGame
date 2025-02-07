#include "RespawnManager.h"
#include "EnemyManager.h"
#include "CollectibleManager.h"
#include "Player.h"

RespawnManager::RespawnManager(const std::shared_ptr<Player>& player, const std::shared_ptr<EnemyManager>& enemyManager, const std::shared_ptr<CollectibleManager>& collectibleManager):
    player(player), enemyManager(enemyManager), collectibleManager(collectibleManager)
{}

void RespawnManager::RespawnAllEntities()
{
    player->HandleRespawn();
    enemyManager->RespawnEnemies();

    auto enemyList = enemyManager->GetAliveEnemies();
    for(const auto& enemy: enemyList)
    {
        EnemyArrow* enemyArrow = dynamic_cast<EnemyArrow*>(&enemy.get());
        if(enemyArrow)
        {
            enemyArrow->ClearArrowPool();
        }
    }

    collectibleManager->ClearCollectibles();

}
