#include "RespawnManager.h"

RespawnManager::RespawnManager(const std::shared_ptr<Player>& player, const std::shared_ptr<EnemyManager>& enemyManager, const std::shared_ptr<CollectibleManager>& collectibleManager, const std::shared_ptr<Camera>& camera, const std::shared_ptr<ProjectileBar>& projectileBar):
    player(player), enemyManager(enemyManager), collectibleManager(collectibleManager), camera(camera), projectileBar(projectileBar)
{}

void RespawnManager::RespawnAllEntities()
{
    player->HandleRespawn();
    enemyManager->RespawnEnemies();
    projectileBar->ResetBar();

    auto enemyList = enemyManager->GetAliveEnemies();
    for(const auto& enemy: enemyList)
    {
        auto enemyArrow = dynamic_cast<EnemyArrow*>(&enemy.get());
        if(enemyArrow)
        {
            enemyArrow->ClearArrowPool();
        }
    }

    collectibleManager->ClearCollectibles();
    camera->Reset();
}
