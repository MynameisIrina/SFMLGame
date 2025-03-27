#pragma once
#include "player/Player.h"
#include "managers/EnemyManager.h"
#include "managers/CollectibleManager.h"
#include "ui/ProjectileBar.h"

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