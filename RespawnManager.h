#pragma once
#include "Player.h"
#include "EnemyManager.h"
#include "CollectibleManager.h"
#include "ProjectileBar.h"

class RespawnManager
{


public:
    RespawnManager(const std::shared_ptr<Player>& player,
                   const std::shared_ptr<EnemyManager>& enemyManager,
                   const std::shared_ptr<CollectibleManager>& collectibleManager,
                   const std::shared_ptr<Camera>& camera,
                   const std::shared_ptr<ProjectileBar>& projectileBar);

    void RespawnAllEntities();

private:
    std::shared_ptr<Player> player;
    std::shared_ptr<EnemyManager> enemyManager;
    std::shared_ptr<CollectibleManager> collectibleManager;
    std::shared_ptr<Camera> camera;
     std::shared_ptr<ProjectileBar> projectileBar;

};